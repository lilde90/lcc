#include "atom.h"
#include <limit.h>
#include <string.h>
#include "assert.h"
#define NELEMS(x) ((sizeof (x)) / (sizeof ((x)[0])))
static struct atom {
    struct atom *link;
    struct len;
    char *str;
} *buckets[2048];

const char *atom_string(const char *str) {
    assert(str);
    return atom_new(str, strlen(str));
}
