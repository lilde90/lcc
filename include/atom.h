#ifndef ATOM_H
#define ATOM_H
extern int atom_len(const char *str);
extern const char* atom_new(const char *str, int len);
extern const char* atom_string(const char *str);
extern const char* atom_int(long n);

#endif
