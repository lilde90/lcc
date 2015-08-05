// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <stdarg.h>
#include <string.h>
#include "assert.h"
#include "bit.h"
#include "mem.h"

#define T Bit_T

struct T {
    int length;
    unsigned char *bytes;
    unsigned long *words;
};

#define BPW (8*sizeof(unsigned long))
#define nwords(len) ((((len) + BPW - 1)&(~(BPW-1)))/BPW)
#define nbytes(len) ((((len) + 8 - 1) &(~(8-1)))/8)

T Bit_new(int length) {
    T bit;
    assert(length > 0);
    NEW(bit);
    if (length > 0) {
        bit->words = CALLOC(nwords(length), sizeof(unsigned long));
    } else {
        bit->words = NULL;
    }
    bit->bytes = (unsigned char *)bit->words;
    bit->length = length;
    return bit;
}

int Bit_length(T set) {
    assert(set);
    return set->length;
}

void Bit_free(T *set) {
    assert(set && *set);
    FREE((*set)->words);
    FREE(*set);
}
