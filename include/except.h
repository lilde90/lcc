#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include <setjmp.h>

#define T Except_T
typedef struct T {
    const char *reason;
} T;
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
void Except_raise(const T* e, const char* file, int line);

typedef struct Except_Frame Except_Frame;
struct Except_Frame {
    Except_Frame *prev;
    jmp_buf env;
    const char *file;
    int line;
    const T *exception;
};
extern Except_Frame *except_stack;
#define RERAISE Except_raise(Except_frame.exception, \
        Except_frame.file, Except_frame.line)
#define RETURN switch (Except_stack = Except_stack->prev, 0) default: return
#undef T
#endif
