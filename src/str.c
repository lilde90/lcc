// Copyright 2015 lilde90. All Rights Reserved.
// Author: Pan Li (panli.me@gmail.com)
//
#include <string.h>
#include <limits.h>
#include "assert.h"
#include "mem.h"
#include "str.h"

#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1);
#define convert(s, i, j) do { int len; \
    assert(s); len = strlen(s); \
    i = idx(i, len); j = idx(j, len); \
    if (i > j) { int tmp = i; i =  j; j = tmp; } \
    assert(i >= 0 && j <= len); } while(0)

char *str_sub(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    char *str = ALLOC(j - i + 1);
    char *p = str;
    while (i < j) {
        *str++ = s[i++];
    } 
    *str = '\0';
    return p;
}

char *str_dup(const char *s, int i, int j, int n) {
    assert(s);
    assert(n >= 0);
    convert(s, i, j);
    char *str = ALLOC(n * (j - i) + 1);
    char *p = str;
    if (j > i) {
        while (n-- > 0) {
            for (int idx = i; idx < j; idx++) {
                *str++ = s[idx];
            }
        }
    }
    *str = '\0';
    return p;
}

char *str_cat(const char *s1, int i1, int j1,
        const char *s2, int i2, int j2) {
    assert(s1 && s2);
    convert(s1, i1, j1);
    convert(s2, i2, j2);
    char *str = ALLOC(j1 - i1 + j2 - i2 + 1);
    char *p = str;
    while (i1 < j1) {
        *str++ = s1[i1++];
    }
    while (i2 < j2) {
        *str++ = s2[i2++];
    }
    *str = '\0';
    return p;
}

char *str_catv(const char *s, ...) {
    char *str, *p;
    const char *save = s;
    va_list ap;
    int i, j, len;
    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        len += j - i;
        s = va_arg(ap ,const char*);
    }
    va_end(ap);
    p = str = ALLOC(len + 1);
    s = save;
    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        len += j - i;
        while (i < j) {
            *p++ = s[i++];
        }
        s = va_arg(ap, const char*);
    }
    va_end(ap);
    *p = '\0';
    return str;
}

char *str_reverse(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    char *str = ALLOC(j -i + 1);
    char *p = str;
    while (j > i) {
        *str++ = s[--j];
    }
    *str = '\0';
    return p;
}

char *str_map(const char *s, int i, int j,
        const char *from, const char *to) {
    static char map[256] = { 0 };
    if (from && to) {
        unsigned c;
        for (c = 0; c < sizeof map; c++) {
            map[c] = c;
        }
        while (*from && *to) {
            map[(unsigned char)*from++] = *to++;
        }
        assert(*from == 0 && *to == 0);
    } else {
        assert(from == NULL && to == NULL && s);
        assert(map['a']);
    }

    if (s) {
        char *str, *p;
        convert(s, i, j);
        p = str= ALLOC(j - i + 1);
        while (i < j) {
            *p++ = map[(unsigned char)s[i++]];
        }
        *p = '\0';
        return str;
    } else {
        return NULL;
    }
}

int str_pos(const char *s, int i) {
    assert(s);
    int len = strlen(s);
    i = idx(i, len);
    assert(i >= 0 && i <= len);
    return i + 1;
}

int str_len(const char *s, int i, int j) {
    assert(s);
    convert(s, i, j);
    return j - i;
}

int str_cmp(const char *s1, int i1, int j1,
        const char *s2, int i2, int j2) {
    convert(s1, i1, j1);
    convert(s2, i2, j2);
    s1 += i1;
    s2 += i2;
    if (j1 -i1 < j2 -i2) {
        int cond = strncmp(s1, s2, j1 - i1);
        return cond == 0 ? -1 : cond;
    } else if (j1 - i1 > j2 -i2) {
        int cond = strncmp(s1, s2, j2 - i2);
        return cond == 0 ? +1 : cond;
    } else {
        return strncmp(s1, s2, j1 - i1);
    }
}
    
int str_chr(const char *s, int i, int j, int c) {
    assert(s);
    convert(s, i, j);
    for (int idx = i; idx < j; idx++) {
        if (s[idx] == c) {
            return idx + 1;
        }
    }
    return 0;
}

int str_rchr(const char *s, int i, int j, int c) {
    assert(s);
    convert(s, i, j);
    for (int idx = j - 1; idx >= i; idx--) {
        if (s[idx] == c) {
            return idx + 1;
        }
    }
    return 0;
}

int str_upto(const char *s, int i, int j,
        const char *set) {
    assert(set);
    convert(s, i, j);
    for (int idx = i; idx < j; idx++) {
        if (strchr(set, s[idx])) {
            return idx + 1;
        }
    }
    return 0;
}

int str_rupto(const char *s, int i, int j,
        const char *set) {
    assert(set);
    convert(s, i, j);
    for (int idx = j - 1; idx >= i; idx--) {
        if (strrchr(set, s[idx])) {
            return idx + 1;
        }
    }
    return 0;
}

int str_find(const char *s, int i, int j,
        const char *str) {
    assert(str);
    convert(s, i, j);
    int len = strlen(str);
    if (len == 0) {
        return i + 1;
    } else if (len == 1) {
        for (int idx = i; idx < j; idx++) {
            if (s[idx] == *str) {
                return idx + 1;
            }
        }
    } else {
        for (int idx = i; idx + len <= j; idx++) {
            if (strncmp(&s[idx], str, len) == 0) {
                return idx + 1;
            }
        }
    }
    return 0;
}

int str_rfind(const char *s, int i, int j,
        const char *str) {
    assert(str);
    convert(s, i, j);
    int len = strlen(str);
    if (len == 0) {
        return i + 1;
    } else if (len == 1) {
        for (int idx = j - 1; idx >= i; idx--) {
            if (s[idx] == *str) {
                return idx + 1;
            }
        }
    } else {
        for (int idx = j; idx - len >= i; idx--) {
            if (strncmp(&s[idx - len], str, len) == 0) {
                return idx - len + 1;
            }
        }
    }
    return 0;
}

int str_any(const char *s, int i,
        const char *set) {
    assert(set);
    assert(s);
    int len = strlen(s);
    i = idx(i, len);
    assert(i >= 0 && i <= len);
    if (i < len && strchr(set, s[i])) {
        return i + 2;
    }
    return 0;
}

int str_many(const char *s, int i, int j,
        const char *set) {
    assert(set);
    convert(s, i, j);
    if (i < j && strchr(set, s[i])) {
        do {
            i++;
        } while(i < j && strchr(set, s[i]));
        return i + 1;
    }
    return 0;
}

int str_rmany(const char *s, int i, int j,
        const char *set) {
    assert(set);
    convert(s, i, j);
    if (j > i && strchr(set, s[j - 1])) {
        do {
            --j;
        } while(j >= i && strchr(set, s[j]));
        return j + 2;
    }
    return 0;
}

int str_match(const char *s, int i, int j,
        const char *str) {
    assert(str);
    convert(s, i, j);
    int len = strlen(str);
    if (len == 0) {
        return i + 1;
    } else if (len == 1) {
        if (i < j && s[i] == *str) {
            return i + 2;
        }
    } else if (i + len <= j && strncmp(&s[i], str, len) == 0) {
        return i + len + 1;
    }
    return 0;
}

int str_rmatch(const char *s, int i, int j,
        const char *str) {
    assert(str);
    convert(s, i, j);
    int len = strlen(str);
    if (len == 0) {
        return j + 1;
    } else if (len == 1) {
        if (j > i && s[j - 1] == *str) {
            return j;
        }
    } else if (j - len >= i && strncmp(&s[j - len], str, len) == 0) {
        return j - len + 1;
    }
    return 0;
}

void str_fmt(int code, va_list *app,
        int put(int c, void* cl), void *cl,
        unsigned char flags[], int width, int precision) {
    char *s;
    int i, j;
    assert(app && flags);
    s = va_arg(*app, char *);
    i = va_arg(*app, int);
    j = va_arg(*app, int);
    convert(s, i, j);
}
