// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <stdlib.h>
#include <stdarg.h>
#include "ring.h"
#include "assert.h"
#include "mem.h"

#define T Ring_T
struct T {
    struct node {
        struct node *llink, *rlink;
        void *value;
    } *head;
    int length;
};

T Ring_new(void) {
    T ring;
    NEW0(ring);
    ring->head = NULL;
    return ring;
}

T Ring_ring(void *x, ...) {
    va_list ap;
    T ring = Ring_new();
    va_start(ap, x);
    for (; x; x = va_arg(ap, void *)) {
        Ring_addhi(ring, x);
    }
    va_end(ap);
    return ring;
}

void Ring_free(T *ring) {
    assert(ring && *ring);
    struct node *p;

    if ((p = (*ring)->head) != NULL) {
        int n = (*ring)->length;
        struct node *q = NULL;
        for(; n-- > 0; p = q) {
            q = p->rlink;
            FREE(p);
        }
    }

    FREE(*ring);
}

int Ring_length(T ring) {
    assert(ring);
    return ring->length;
}

extern void *Ring_get(T ring, int i) {
    assert(ring);
    assert(i >= 0 && i < ring->length);
    struct node *p = ring->head;
    if (i <= ring->length / 2) {
        int n = i;
        while (n-- > 0) {
            p = p->rlink;
        }
    } else {
        int n = ring->length - i;
        while (n-- > 0) {
            p = p->llink;
        }
    }

    return p->value;
}

extern void *Ring_put(T ring, int i, void *x) {
    assert(ring);
    assert(i >=0 && i < ring->length);
    struct node *p = ring->head;
    if (i <= ring->length / 2) {
        int n = i;
        while (n-- > 0) {
            p = p->rlink;
        }
    } else {
        int n = ring->length - i;
        while (n-- > 0) {
            p = p->llink;
        }
    }

    void *prev = p->value;
    p->value = x;
    return prev;
}

void *Ring_add(T ring, int pos, void *x) {
    assert(ring);
    pos = (pos % ring->length);
    struct node *p;
    NEW(p);
    struct node *q = ring->head;
    if (pos == 1) {
        return Ring_addlo(ring, x);
    } else if (pos == ring->length) {
        return Ring_addhi(ring, x);
    } else {
        int i = pos - 1; 
        struct node *q = ring->head;
        if (i <= ring->length / 2) {
            int n = i;
            while (n-- > 0) {
                q = q->rlink;
            }
        } else {
            int n = ring->length - i;
            while (n-- > 0) {
                q = q->llink;
            }
        }
        q->llink->rlink = p;
        p->rlink = q;
        p->llink = q->llink;
        q->llink = p;
        ring->length++;
        return p->value = x;
    }

}

void *Ring_addlo(T ring, void *x) {
    assert(ring);
    struct node *p;
    NEW(p);
    struct node *q = ring->head;
    if (q == NULL) {
        ring->head = p;
        p->llink = p->rlink = p;
    } else {
        q->llink->rlink = p;
        p->rlink = q;
        p->llink = q->llink;
        q->llink = p;
    }
    ring->head = p;
    ring->length++;
    return p->value = x;
}

void *Ring_addhi(T ring, void *x) {
    assert(ring);
    struct node *p;
    NEW(p);
    p->value = x;
    struct node *q = ring->head;
    if (q == NULL) {
        ring->head = p;
        p->llink = p->rlink = p;
    } else {
        q->llink->rlink = p;
        p->rlink = q;
        p->llink = q->llink;
        q->llink = p;
    }
    ring->length++;
    return p->value = x;
}

void *Ring_remove(T ring, int i) {
    assert(ring);
    assert(ring->length > 0);
    assert(i >= 0 && i < ring->length);
    struct node *p = ring->head;
    if (i <= ring->length / 2) {
        int n = i;
        while (n-- > 0) {
            p = p->rlink;
        }
    } else {
        int n = ring->length - i;
        while (n-- > 0) {
            p = p->llink;
        }
    }
    if (i == 0) {
        ring->head = ring->head->rlink;
    }
    void *x = p->value;
    p->llink->rlink = p->rlink;
    p->rlink->llink = p->llink;
    FREE(p);
    ring->length--;
    if (ring->length == 0) {
        ring->head = NULL;
    }
    return x;
}

void *Ring_remlo(T ring) {
    assert(ring);
    assert(ring->length > 0);
    struct node *p = ring->head;
    void *x = p->value;
    p->llink->rlink = p->rlink;
    p->rlink->llink = p->llink;
    FREE(p);
    ring->length--;
    if (ring->length == 0) {
        ring->head = NULL;
    } else {
        ring->head = p->rlink;
    }

    return x;
}

void *Ring_remhi(T ring) {
    assert(ring);
    assert(ring->length > 0);
    struct node *p = ring->head->llink;
    void *x = p->value;
    p->llink->rlink = p->rlink;
    p->rlink->llink = p->llink;
    FREE(p);
    ring->length--;
    if (ring->length == 0) {
        ring->head = NULL;
    }
    return x;
}

void Ring_rotate(T ring, int n) {
    assert(ring);
    assert(n >= -ring->length && n <= ring->length);
    int i ;
    if (n >= 0) {
        i = n % ring->length;
    } else {
        i = n + ring->length;
    }
    struct node *p = ring->head;
    if (i <= ring->length / 2) {
        int n = i;
        while (n-- > 0) {
            p = p->rlink;
        }
    } else {
        int n = ring->length - i;
        while (n-- > 0) {
            p = p->llink;
        }
    }
    ring->head = p;
}
