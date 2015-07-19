// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <limits.h>
#include <stddef.h>
#include "mem.h"
#include "assert.h"
#include "table.h"

#define T Table_T

struct T {
    int size;
    int length;
    unsigned timestamp;
    int (*cmp)(const void *, const void *);
    unsigned (*hash)(const void *key);
    struct binding {
        struct binding *link;
        const void *key;
        void *value;
    } **buckets;
};
    
static int default_cmp(const void *x, const void *y) {
    return x != y;
}

static unsigned default_hash(const void *key) {
    return (unsigned long)key >> 2;
}
void *Table_get(T table, const void *key) {
    assert(table);
    assert(key);
    unsigned hash = (*table->hash)(key) % table->size;
    struct binding *head = NULL;
    for(head = table->buckets[hash]; head; head = head->link) {
        if ((*table->cmp)(head->key, key) == 0) {
            return head->value;
        }
    }
    return NULL;
}

void *Table_put(T table, const void *key, void *value) {
    assert(table);
    assert(key);
    unsigned hash = (*table->hash)(key) % table->size;
    struct binding *head = NULL;
    struct binding *prev = NULL;
    for (head = table->buckets[hash]; head; head = head->link) {
        if ((*table->cmp)(head->key, key) == 0) {
            break;
        }
    }
    if (head == NULL) {
        NEW(head);
        head->key = key;
        head->link = table->buckets[hash];
        table->buckets[hash] = head;
        table->length++;
        prev = NULL;
    } else {
        prev = head->value;
    }
    head->value = value;
    table->timestamp++;
    return prev;
}

T Table_new(int hint, 
        int cmp(const void *x, const void *y),
        unsigned hash(const void *key)) {
    T table;
    int i = 1;
    assert(hint >= 0);

    static int primes[] = {509, 509, 1021, 2053, 4093,
        8191, 16381, 32771, 65521, INT_MAX};
    while (primes[i] < hint) {
        i++;
    }
    table = ALLOC(sizeof(*table) + primes[i-1] * sizeof (table->buckets[0]));
    table->size = primes[i-1];
    table->cmp = cmp ? cmp : default_cmp;
    table->hash = hash ? hash :default_hash;
    table->buckets = (struct binding **)(table + 1);
    for (i = 0; i < table->size; i++) {
        table->buckets[i] = NULL;
    }
    table->length = 0;
    table->timestamp = 0;
    return table;
}

int Table_length(T table) {
    assert(table);
    return table->length;
}

void **Table_toArray(T table, void *end) {
    int i = 0, j = 0;
    void **array;
    struct binding *p;
    assert(table);

    array = ALLOC((2 * table->length + 1) * sizeof (*array));
    for (; i < table->size; i++) {
        for (p = table->buckets[i]; p; p = p->link) {
            array[j++] = (void *)p->key;
            array[j++] = p->value;
        }
    }
    array[j] = end;
    return array;
}

void Table_free(T *table) {
    assert(table && *table);

    if ((*table)->length > 0) {
        int i = 0;
        struct binding *p = NULL, *q = NULL;
        for (; i < (*table)->size; i++) {
            for (p = (*table)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
        }
        FREE(*table);
    }
}

void Table_map(T table,
        void apply(const void *key, void **value, void *cl),
        void *cl) {
    assert(table);
    assert(apply);

    int i = 0;
    unsigned stamp = table->timestamp;
    struct binding *p = NULL;

    for (; i < table->size; i++) {
        for (p = table->buckets[i]; p; p = p->link) {
            apply(p->key, &(p->value), cl);
            assert(stamp == table->timestamp);
        }
    }
}

void *Table_remove(T table, const void *key) {
    assert(table);
    assert(key);
    int hash = (*table->hash)(key) % (table->size);
    struct binding **pp = NULL;
    for (pp = &table->buckets[hash]; *pp; pp = &(*pp)->link) {
        if ((*table->cmp)(key, (*pp)->key) == 0) {
            struct binding *p = *pp;
            void *value = p->value;
            *pp = p->link;
            FREE(p);
            table->length--;
            return value;
        }
    }
    return NULL;
}
