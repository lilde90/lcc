// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//

#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "assert.h"

#define T Seq_T
struct T {
    struct ARRAY_T array;
    int length;
    int head;
};

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

