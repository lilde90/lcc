// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <string.h>
#include <limits.h>
#include "assert.h"
#include "mem.h"
#include "str.h"

#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1);
#define convert(s, i, j) do { int len; \
    assert(s); len = strlen(s); \
    i = idx(i, len); j = idx(j, len); \
    if (i > j) { int tmp = i; i =  j; j = tmp; } \
    assert(i >= 0 && j <= len); } while(0)

char *str_sub(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    char *str = ALLOC(j - i + 1);
    char *p = str;
    while (i < j) {
        *str++ = s[i++];
    } 
    *str = '\0';
    return p;
}

char *str_dup(const char *s, int i, int j, int n) {
    assert(s);
    assert(n >= 0);
    convert(s, i, j);
    char *str = ALLOC(n * (j - i) + 1);
    char *p = str;
    if (j > i) {
        while (n-- > 0) {
            for (int idx = i; idx < j; idx++) {
                *str++ = s[idx];
            }
        }
    }
    *str = '\0';
    return p;
}

char *str_cat(const char *s1, int i1, int j1,
        const char *s2, int i2, int j2) {
    assert(s1 && s2);
    convert(s1, i1, j1);
    convert(s2, i2, j2);
    char *str = ALLOC(j1 - i1 + j2 - i2 + 1);
    char *p = str;
    while (i1 < j1) {
        *str++ = s1[i1++];
    }
    while (i2 < j2) {
        *str++ = s2[i2++];
    }
    *str = '\0';
    return p;
}

char *str_catv(const char *s, ...) {
    return NULL;
}

char *str_reverse(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    char *str = ALLOC(j -i + 1);
    char *p = str;
    while (j > i) {
        *str++ = s[--j];
    }
    *str = '\0';
    return p;
}

char *str_map(const char *s, int i, int j,
        const char *from, const char *to) {
    return NULL;
}

int str_pos(const char *s, int i) {
    assert(s);
    int len = strlen(s);
    i = idx(i, len);
    assert(i >= 0 && i <= len);
    return i + 1;
}

int str_len(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    return j - i;
}
    
