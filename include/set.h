// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#ifndef SET_INCLUDED
#define SET_INCLUDED
#define T Set_T
typedef struct T *T;

extern T Set_new(int hint, 
        int mcp(const void *x, const void *y),
        unsigned hash(const void *key));
extern void Set_free(T *set);

extern int Set_length(T set);
extern int Set_member(T set, const void *member);
extern void Set_put(T set, const void *member);
extern void *Set_remove(T set, const void *member);

extern void Set_map(T set,
        void apply(const void *member, void *cl),
        void *cl);
extern void **Set_toArray(T set, void *end);

extern T Set_union(T s, T t);
extern T Set_inter(T s, T t);
extern T Set_minus(T s, T t);
extern T Set_diff(T s, T t);

static int default_cmp(const void *x, const void *y);
static unsigned default_hash(const void *x);
static T copy(T t, int hint);
#undef T
#endif
