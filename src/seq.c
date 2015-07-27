// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//

#include <stdlib.h>
#include <string.h>
#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "assert.h"
#include "mem.h"

#define T Seq_T
struct T {
    struct ARRAY_T array;
    int length;
    int head;
};

T Seq_new(int hint) {
    T seq;
    assert(hint >= 0);
    NEW0(seq);
    if (hint == 0) {
        hint = 16;
    }
    ArrayRep_init(&seq->array, hint, sizeof(void *), ALLOC(hint*sizeof(void *)));
    return seq;
}

void Seq_free(T *seq) {
    assert(seq && *seq);
    assert((void *)*seq == (void *)&(*seq)->array);
    Array_free((ARRAY_T*)seq);
}

int Seq_length(T seq) {
    assert(seq);
    return seq->length;
}

void *Seq_get(T seq, int i) {
    assert(seq);
    assert(i >=0 && i < seq->length);
    return ((void **)seq->array.array)[(seq->head + i) % (seq->length)];
}

void *Seq_put(T seq, int i, void *x) {
    assert(seq);
    assert(i >= 0 && i < seq->length);
    void *prev = ((void **)seq->array.array)[(seq->head + i) % (seq->length)];
    ((void **)seq->array.array)[(seq->head + i) % (seq->length)] = x;
    return prev;
}

void *Seq_addlo(T seq, void *x) {
    assert(seq);
    if (seq->length == seq->array.length) {
        expand(seq);
    }

    if (seq->head-- < 0) {
        seq->head = seq->array.length - 1;
    }

    seq->head = (seq->head) %(seq->length++);
    return ((void **)seq->array.array)[(seq->head) % (seq->length)] = x;
}

void *Seq_addhi(T seq, void *x) {
    assert(seq);
    if (seq->length == seq->array.length) {
        expand(seq);
    }
    
    int i = seq->length++;
    return ((void **)seq->array.array)[(seq->head + i) % (seq->length)] = x;
}

void *Seq_remlo(T seq) {
    assert(seq);
    assert(seq->length > 0);
    void *x = ((void **)seq->array.array)[(seq->head) % (seq->length)];
    seq->head = (seq->head + 1) % (seq->length--);
    return x;
}

void *Seq_remhi(T seq) {
    assert(seq);
    assert(seq->length > 0);
    int i = --seq->length;
    return ((void **)seq->array.array)[(seq->head + i) % (seq->length)];
}

void expand(T seq) {
    assert(seq);

    Array_resize(&seq->array ,seq->array.length * 2);
    void **old = &((void **)seq->array.array)[seq->head];
    memcpy(old + seq->array.length, old, (seq->array.length - seq->head) * sizeof(void **));
    seq->head += seq->array.length;
}
