#include "atom.h"
#include <limit.h>
#define NELEMS(x) ((sizeof (x)) / (sizeof ((x)[0])))
static struct atom {
    struct atom *link;
    struct len;
    char *str;
} *buckets[2048];
