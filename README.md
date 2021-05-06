# generic-c
A header-only C11 library with common generic datastructures.

To use, simply add the `include` directory to your include directories and include any of the `lk/gen/*` headers (see [how to use](#how-to-use) for more info).

Currently implemented and tested: 

- `lk/gen/stack.h` - LIFO datastructure.
- `lk/gen/queue.h` - FIFO datastructure.
- `lk/gen/vector.h` - Random-Access dynamic size datastructure.

## How to use

These are generic headers, so they allow you to define a stack, queue, etc. of any type.

This is done as follows:

```c
// define your datatype
#define T int
#include <lk/gen/stack.h>
#undef T
```

Now you will be able to use `lk_stack_int_*` functions and types. The resulting type names and function names are always of the format `lk_stack_T_...` where T is the typename.
For `T` as `float`, these would be `lk_stack_float` and so on.

Note that `T` has to be a purely alphanumeric typename with no spaces. If you want to use a struct, you should typedef it like so:

```c
// example data
struct Employee {
  char name[64];
  unsigned char age;
};

typedef struct Employee employee;

// note that we're using the typedef here, as "struct Employee" would have a space in it.
#define T employee
#include <lk/gen/stack.h>
#undef T
```

Now, a structure names `lk_stack_employee` exists and matching `lk_stack_employee_*` functions do as well.

Internally, `sizeof(T)` is called to determine space and indexing in the data structure.


