// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#ifndef ARRAYREP_INCLUDED
#define ARRAYREP_INCLUDED

#define T ARRAY_T

struct T {
    int length;
    int size;
    char *array;
};

extern void ArrayRep_init(T array, int length, int size,
        void *arr);

#undef T
#endif
