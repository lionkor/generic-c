#define T int
#include <lk/gen/stack.h>
#undef T

typedef struct {
    int a;
    float b;
} mystruct;

#define T mystruct
#include <lk/gen/stack.h>
#undef T

#define T int
#include <lk/gen/queue.h>
#undef T

#define T mystruct
#include <lk/gen/queue.h>
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

lk_iteration_decision print_int(int* i) {
    printf("%d\n", *i);
    return LK_CONTINUE;
}

void test_stack(void) {
    {
        register_case("stack init");
        lk_stack_int stack;
        check(lk_stack_int_init(&stack));
        check(stack.capacity == 1);
        check(stack.size == 0);
        check(stack.data != NULL);
        lk_stack_int_free(&stack);
    }
    {
        register_case("stack init with size");
        lk_stack_int stack;
        check(lk_stack_int_init_with_size(&stack, 10));
        check(stack.capacity == 10);
        check(stack.size == 0);
        check(stack.data != NULL);
        lk_stack_int_free(&stack);
    }
    {
        register_case("stack push");
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
            register_case("stack pop");
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
}

void test_queue(void) {
    {
        register_case("queue init");
        lk_queue_int q;
        check(lk_queue_int_init(&q));
        check(q.size == 0);
        lk_queue_int_free(&q);
    }
    {
        register_case("queue push");
        lk_queue_int q;
        check(lk_queue_int_init_with_size(&q, 100));
        check(q.size == 0);
        int x = 35;
        for (size_t i = 0; i < 100; ++i) {
            printf("%u: r=%u, w=%u\n", (unsigned)i, (unsigned)q.readi, (unsigned)q.writei);
            check(lk_queue_int_push(&q, &x));
            if (i % 5 == 0) {
                check(lk_queue_int_pop(&q, &x));
            }
        }
        lk_queue_int_free(&q);
    }
}

int main(void) {
    test_stack();
    test_queue();
    return passed();
}
