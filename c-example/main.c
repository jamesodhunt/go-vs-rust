#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "foo.h"

const char *program_name;

int
test_foo(const char *name, const char *age)
{
    Foo *foo = NULL;

    foo = new_foo(name, age); 
    if (!foo) {
        fprintf(stderr, "ERROR: failed to create a Foo\n");
        return -1;
    }

    printf("Foo: name: '%s', age: %u\n", foo->name, foo->age);

    free_foo(foo);

    return 0;
}

int
main(int argc, char *argv[])
{
    program_name = argv[0];

    if (argc != 3) {
        fprintf(stderr, "usage: %s <name> <age>\n", program_name);
        exit(EXIT_FAILURE);
    }

    const char *name = argv[1];
    const char *age = argv[2];

    int ret = test_foo(name, age);

    exit(ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
