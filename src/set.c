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

T Set_union(T s, T t) {
    if (t == NULL) {
        assert(s);
        return copy(s, s->size);
    } 
    if (s == NULL) {
        assert(t);
        return copy(t, t->size);
    } 
    T set = copy(s, s->size > t->size ? s->size : t->size);
    assert(s->cmp == t->cmp && s->hash == t->hash);
    int i;
    struct member *p = NULL;
    for (int i = 0; i < t->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            const void *member = p->member;
            Set_put(set, member);
        }
    return set;
    }
}

T Set_inter(T s, T t) {
    if (s == NULL) {
        assert(t);
        return copy(t, t->size);
    }
    if (t == NULL) {
        assert(s);
        return copy(s, s->size);
    }
    if (s->length < t->length) {
        return Set_inter(t, s);
    } else {
        T set = Set_new(s->size < t->size ? s->size : t->size, s->cmp, t->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        int i;
        struct member *p = NULL;
        for (i = 0; i < t->size; i++) {
            for (p = t->buckets[i]; p; p = p->link) {
                if (Set_member(s, p->member)) {
                    struct member *q = NULL;
                    int hash = (*set->hash)(p->member) % set->size;
                    NEW(q);
                    q->member = p->member;
                    q->link = set->buckets[hash];
                    set->buckets[hash] = q;
                }
            }
        }
        return set;
    }
}

T Set_minus(T s, T t) {
    if (s == NULL) {
        assert(t);
        return Set_new(t->size, t->cmp, t->hash);
    }
    if (t == NULL) {
        assert(s);
        return copy(s->size, s->cmp, s->hash);
    }
    T set = Set_new(s->size < t->size ? s->size : t->size, t->cmp, t->hash);
    assert(s->cmp == t->cmp && s->hash == t->hash);
    int i;
    struct member *p = NULL;
    for (i = 0; i < s->size; i++) {
        for (p = s->buckets[i]; p; p = p->link) {
            if (!Set_member(t, p->member)) {
                struct member *q = NULL;
                int hash = (*set->hash)(p->member) % set->size;
                NEW(q);
                q->member = p->member;
                q->link = set->buckets[hash];
                set->buckets[hash] = q;
            }
        }
    }
    return set;
}
        
T Set_diff(T s, T t) {
    if (s == NULL) {
        assert(t);
        return copy(t->size, t->cmp, t->hash);
    }
    if (t == NULL) {
        assert(s);
        return copy(s->size, s->cmp, s->hash);
    }

    T set = Set_new(s->size < t->size ? s->size : t->size, t->cmp, t->hash);
    assert(s->cmp == t->cmp && s->hash == t->hash);
    
    int i;
    struct member *p = NULL;
    // s-t
    for (i = 0; i < s->size; i++) {
        for (p = s->buckets[i]; p; p = p->link) {
            if (!Set_member(t, p->member)) {
                struct member *q = NULL;
                int hash = (*set->hash)(p->member) % set->size;
                NEW(q);
                q->member = p->member;
                q->link = set->buckets[hash];
                set->buckets[hash] = q;
            }
        }
    }

    // t-s
    for (i = 0; i < t->size; i++) {
        for (p = t->buckets[i]; p; p = p->link) {
            if (!Set_member(s, p->member)) {
                struct member *q = NULL;
                int hash = (*set->hash)(p->member) % set->size;
                NEW(q);
                q->member = p->member;
                q->link = set->buckets[hash];
                set->buckets[hash] = q;
            }
        }
    }
}

static int default_cmp(const void *x, const void *y) {
    return x != y;
}

static unsigned default_hash(const void *x) {
    return (unsigned long)x >> 2;
}

static T copy(T t, int hint) {
    T set;
    assert(t);
    set = Set_new(hint, t->cmp, t->hash);
    struct member *p = NULL;
    int i;
    for (i = 0; i < t->size; i++) {
        for (p = t->buckets[i]; p; p = p->link) {
            struct member *q;
            const void *member = p->member;
            int hash = (*set->hash)(member) % set->size;
            NEW(q);
            q->member = member;
            q->link = set->buckets[hash];
            set->buckets[hash] = q;
        }
    }
    return set;
}
