#define T int
#include <lk/gen/stack.h>
#undef T

#include <stdio.h>

static bool _err = false;
static char* _case = "";
#define register_case(x) _case = x
#define check(x)                                                     \
    do {                                                             \
        bool _ok = (x);                                              \
        if (_ok) {                                                   \
            printf("%s → \x1b[;32msuccess\x1b[;m: %s\n", _case, #x); \
        } else {                                                     \
            printf("%s → \x1b[;31mfailure\x1b[;m: %s\n", _case, #x); \
            _err = true;                                             \
        }                                                            \
    } while (0)

#define passed() _err ? -1 : 0

int print_int(int* i) {
    printf("%d\n", *i);
    return LK_CONTINUE;
}

int main(void) {

    {
        register_case("init");
        lk_stack_int stack;
        check(lk_stack_int_init(&stack));
        check(stack.capacity == 1);
        check(stack.size == 0);
        check(stack.data != NULL);
        lk_stack_int_free(&stack);
    }
    {
        register_case("init with size");
        lk_stack_int stack;
        check(lk_stack_int_init_with_size(&stack, 10));
        check(stack.capacity == 10);
        check(stack.size == 0);
        check(stack.data != NULL);
        lk_stack_int_free(&stack);
    }
    {
        register_case("push");
        lk_stack_int stack;
        check(lk_stack_int_init(&stack));
        check(stack.capacity == 1);
        check(stack.size == 0);
        check(stack.data != NULL);

        int peeked = 0;
        check(!lk_stack_int_peek(&stack, &peeked));
        check(peeked == 0);

        int k = 15;
        check(lk_stack_int_push(&stack, &k));

        check(stack.capacity == 1);
        check(stack.size == 1);
        check(stack.data != NULL);
        check(lk_stack_int_peek(&stack, &peeked));
        check(peeked == 15);

        k = 1000;
        check(lk_stack_int_push(&stack, &k));

        check(stack.capacity > 1);
        check(stack.size == 2);
        check(stack.data != NULL);
        check(lk_stack_int_peek(&stack, &peeked));
        check(peeked == 1000);

        printf("printing entire stack\n");
        lk_stack_int_foreach(&stack, print_int);

        {
            register_case("pop");
            int outvalue = -35;
            check(lk_stack_int_pop(&stack, &outvalue));
            check(stack.size == 1);
            check(outvalue == 1000);
            check(lk_stack_int_pop(&stack, &outvalue));
            check(stack.size == 0);
            check(outvalue == 15);
            check(!lk_stack_int_pop(&stack, &outvalue));
            check(stack.size == 0);
            check(outvalue == 15);
        }
        lk_stack_int_free(&stack);
    }

    return passed();
}
