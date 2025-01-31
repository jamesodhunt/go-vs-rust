/*------------------------------------------------------------------*/
/* Comments                                                         */
/*------------------------------------------------------------------*/
 
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "foo.h"

/* A bit hopeful, maybe ;) */
#define AGE_MAX        120

void
free_foo(Foo *foo)
{
    assert(foo);
    assert(foo->name);

    free(foo->name);
    free(foo);
}

Foo *
new_foo(const char *name, const char *age_str)
{
    int age;
    Foo *foo = NULL;

    if (!name) {
        fprintf(stderr, "ERROR: need non blank name\n");
        return NULL;
    }

    if (!age_str) {
        fprintf(stderr, "ERROR: need non blank age\n");
        return NULL;
    }

    age = atoi(age_str);

    /* Check hard limits */
    if (age <= 0 || age >= CHAR_MAX) {
        fprintf(stderr, "ERROR: invalid age\n");
        return NULL;
    }

    /* Check soft limit */
    if (age > AGE_MAX ) {
        fprintf(stderr, "ERROR: nobody's that old!\n");
        return NULL;
        
    }

    foo = calloc(1, sizeof(Foo));
    if (!foo) {
        fprintf(stderr, "ERROR: failed to allocate space for Foo\n");
        return NULL;
    }

    foo->name = strdup(name);
    if (!foo->name) {
        fprintf(stderr, "ERROR: failed to allocate space for name\n");
        goto error;
    }

    foo->age = age;

    return foo;

error:
    if (foo->name) {
        free(foo->name);
    }

    if (foo) {
        free(foo);
    }

    return NULL;
}
