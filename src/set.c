// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include "set.h"
#include "limits.h"
#include "mem.h"
#include "assert.h"
#include "stddef.h"
#define T Set_T
struct T {
    int length;
    int timestamp;
    int size;
    int (*cmp)(const void *x, const void *y);
    unsigned (*hash)(const void *x);
    struct member {
        struct member *link;
        const void *member;
    } **buckets;
};

T Set_new(int hint,
        int cmp(const void *x, const void *y),
        unsigned hash(const void *x)) {
    assert(hint > 0);
    T ptr;
    int i;
    int bucket_size;
    static int primes[] = {509, 509, 1021, 2053, 4093,
        8191, 16381, 32771, 65521, INT_MAX };
    for (i = 1; primes[i] < hint; i++) {
        ;
    }
    bucket_size = primes[i-1];
    ptr = ALLOC(sizeof(*ptr) + bucket_size * sizeof (struct member *));
    ptr->length = 0;
    ptr->timestamp = 0;
    ptr->size = 0;
    ptr->buckets = (struct member **)(ptr + 1);

    ptr->cmp = cmp ? cmp : default_cmp;
    ptr->hash = hash ? hash : default_hash;
    return ptr;
}

void Set_free(T *set) {
    assert(set && *set);
    if ((*set)->length > 0) {
        struct member *p = NULL;
        struct member *q = NULL;
        int i;
        for (i = 0; i < (*set)->size; i++) {
            for (p = (*set)->buckets[i]; p; q = p->link) {
                q = p->link;
                (*set)->length--;
                FREE(p);
            }
        }
        FREE(*set);
    }
}


int Set_length(T set) {
    assert(set);
    return set->length;
}

int Set_member(T set, const void *member) {
    assert(set && member);
    unsigned hash = (*set->hash)(member) % set->size;
    struct member *p = NULL;
    for (p = set->buckets[hash]; p; p = p->link) {
        if ((*set->cmp)(p->member, member) == 0) {
            break;
        }
    }
    return p != NULL;
}

void Set_put(T set, const void *member) {
    assert(set && member);
    unsigned hash = (*set->hash)(member) % set->size;
    struct member *p = NULL;
    for (p = set->buckets[hash]; p; p = p->link) {
        if ((*set->cmp)(p->member, member) == 0) {
            break;
        }
    }
    if (p == NULL) {
        NEW(p);
        p->member = member;
        p->link = set->buckets[hash];
        set->buckets[hash] = p;
        set->length++;
    } else {
        p->member = member;
    }
    set->timestamp++;
}

void *Set_remove(T set, const void *member) {
    assert(set && member);
    unsigned hash = (*set->hash)(member) % set->size;
    struct member **pp = NULL;
    for (pp = &set->buckets[hash]; *pp; pp = &(*pp)->link) {
        if ((*set->cmp)((*pp)->member, member) == 0) {
            struct member *p = *pp;
            *pp = p->link;
            member = p->link;
            FREE(p);
            set->length--;
            return (void *)member;
        }
    }
    return NULL;
}

void Set_map(T set,
        void apply(const void *member, void *cl),
        void *cl) {
    assert(set && apply && cl);
    int stamp = set->timestamp;
    struct member *p = NULL;
    int i;

    for (i = 0; i < set->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            apply(p->member, cl);
            assert(set->timestamp == stamp);
        }
    }
}

void **Set_toArray(T set, void *end) {
    assert(set);

    void **array = NULL;
    array = ALLOC((set->length) + 1 * sizeof (*array));
    struct member *p = NULL;
    int i, j;

    for (i = 0, j = 0; i < set->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            array[j++] = (void *)p->member;
        }
    }
    array[j] = end;
    return array;
}

static int default_cmp(const void *x, const void *y) {
    return x != y;
}

static unsigned default_hash(const void *x) {
    return (unsigned long)x >> 2;
}
