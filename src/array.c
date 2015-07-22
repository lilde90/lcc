// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#define T ARRAY_T

T Array_new(int length, int size) {
    T array;
    NEW(array);
    if (length == 0) {
        ArrayRep_init(array, length, size, NULL);
    } else {
        ArrayRep_init(array, length, size, CALLOC(length, size));
    }
    return array;
}

void ArrayRep_init(T array, int length, int size, void *arr) {
    assert(array);
    assert(length >= 0);
    assert(size > 0);
    assert(arr && length > 0 || !arr && length == 0);

    array->length = length;
    array->size = size;
    if (length == 0) {
        array->array = NULL;
    } else {
        array->array = arr;
    }
}


void Array_free(T *array) {
    assert(array && *array);
    FREE((*array)->array);
    FREE(array);
}

int Array_length(T array) {
    assert(array);
    return array->length;
}

int Array_size(T array) {
    assert(array);
    return array->size;
}

void *Array_get(T array, int i) {
    assert(array && i >= 0 && i < array->length);
    return (void *)(array->array + i * array->size);
}

void *Array_put(T array, int i, void *elem) {
    assert(array && i >= 0 && i < array->length);
    assert(elem);
    memcpy(array + i * array->size, elem, array->size);
    return elem;
}

void Array_resize(T array, int length) {
    assert(array && length >= 0);
    if (length == 0) {
        FREE(array->array);
    } else if (array->length == 0) {
        array->array = ALLOC(length * array->size);
    } else {
        RESIZE(array->array, length * array->size);
    }
    array->length = length;
}

T Array_copy(T array, int length) {
    assert(array && length >= 0);
    T ptr;
    NEW(ptr);

    if (length == 0) {
        ptr->array = NULL;
    } else if (length > array->length){
        ptr->array = ALLOC(length * array->size);
        memcpy(ptr->array, array->array, array->length * array->size);
    } else {
        ptr->array = ALLOC(length * array->size);
        memcpy(ptr->array, array->array, length * array->size);
    }
    ptr->length = length;
    ptr->size = array->size;
    return ptr;
}
    
