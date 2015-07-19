// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "atom.h"
#include "table.h"
#include "mem.h"
#include "getword.h"

void wf(char *, FILE *);
int compare(const void *, const void *);
void vfree(const void *, void **, void*);
int first(int);
int rest(int);

int compare(const void *x, const void *y) {
    return strcmp(*(char **)x, *(char **)y);
}

void vfree(const void *key, void **count, void *cl) {
    FREE(*count);
}

int first(int c) {
    return isalpha(c);
}

int rest(int c) {
    return isalpha(c) || c == '_';
}

void wf(char *name, FILE *fp) {
    Table_T table = Table_new(0, NULL, NULL);
    char buf[128];
    int i;

    while(getword(fp, buf, sizeof buf, first, rest)) {
        const char *word;
        int *count;
        for (i = 0; buf[i] != '\0'; i++) {
            buf[i] = tolower(buf[i]);
        }
        word = atom_string(buf);
        count = Table_get(table, word);
        if (count) {
            (*count)++;
        } else {
            NEW(count);
            *count = 1;
            Table_put(table, word, count);
        }
    }
    if (name) {
        printf("%s:\n", name);
    }

    void **array = Table_toArray(table, NULL);
    qsort(array, Table_length(table), 2 * sizeof (*array), compare);
    for (i = 0; array[i]; i += 2) {
        printf("%d\t%s\n", *(int *)array[i+1], (char *)array[i]);
    }
    FREE(array);
    Table_map(table, vfree, NULL);
    Table_free(&table);
}


int main(int argc, char **argv) {
    int i;

    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: cannot open %s \n",
                    argv[0], argv[i]);
            return EXIT_FAILURE;
        } else {
            wf (argv[i], fp);
            fclose(fp);
        }
    }

    if (argc == 1) {
        wf(NULL, stdin);
    }
    return EXIT_SUCCESS;
}
