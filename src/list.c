// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <stdarg.h>
#include <stddef.h>
#include "assert.h"
#include "mem.h"
#include "list.h"

#define T List_T

T List_push(T list, void *x) {
    T p;
    NEW(p);
    p->data = x;
    p->next = list;
    return p;
}

T List_list(void *x, ...) {
    va_list ap;
    T list;
    T *p = &list;

    va_start(ap, x);
    for(; x; x = va_arg(ap, void*)) {
        NEW(*p);
        (*p)->next = x;
        p = &(*p)->next;
    }
    *p = NULL;
    va_end(ap);
    return list;
}

T List_append(T list, T tail) {
    T *p = &list;
    while(list->next) {
        p = &(*p)->next;
    }
    *p = tail;
    return list;
}

T List_copy(T list) {
    T head;
    T *p = &head;

    while (list) {
        NEW(*p);
        (*p)->data = list->data;
        p = &(*p)->next;
        list = list->next;
    }
    *p = NULL;
    return head;
}

T List_pop(T list, void **x) {
    if (list == NULL) {
        return list;
    }
    T head = list->next;
    if (x) {
        *x = list->data;
    }
    FREE(list);
    return head;
}

T List_reverse(T list) {
    T head = NULL, next;

    while (list) {
        next = list->next;
        list->next = head;
        head = list; 
        list = next;
    }
    return head;
}

int List_length(T list) {
    int n = 0;
    while (list) {
        n++;
        list = list->next;
    }
    return n;
}

void List_free(T *list) {
    T next;
    assert(list);

    while(*list) {
        next = (*list)->next;
        FREE(*list);
        *list = next;
    }
}

void List_map(T list, void apply(void **x, void *cl), void *cl) {
    assert(apply);
    while (list) {
        apply(&list->data, cl);
        list = list->next;
    }
}

void **List_toArray(T list, void *end) {
    int i;
    int n = List_length(list);
    void **array = ALLOC((n + 1) * sizeof (*array));
    
    for (i = 0; i < n; i++) {
        array[i] = list->data;
        list = list->next;
    }
    array[i] = end;
    return array;
}
