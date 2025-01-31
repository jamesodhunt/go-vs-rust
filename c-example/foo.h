#include <stdint.h>

struct foo {
    char *name;
    uint8_t age;
};

typedef struct foo Foo;

Foo *new_foo(const char *name, const char *age_str);
void free_foo(Foo *foo);
