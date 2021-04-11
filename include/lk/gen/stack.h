#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef T
#warning "T needs to be defined"
#define T int
#endif // T

#define CONCAT_(A, B) A##B
#define CONCAT(A, B) CONCAT_(A, B)
#define CONCAT3_(A, B, C) A##B##C
#define CONCAT3(A, B, C) CONCAT3_(A, B, C)

#define lk_stack CONCAT(lk_stack_, T)
#define lk_stack_init CONCAT3(lk_stack_, T, _init)
#define lk_stack_init_with_size CONCAT3(lk_stack_, T, _init_with_size)
#define lk_stack_free CONCAT3(lk_stack_, T, _free)
#define lk_stack_push CONCAT3(lk_stack_, T, _push)
#define lk_stack_peek CONCAT3(lk_stack_, T, _peek)
#define lk_stack_foreach_callback CONCAT3(lk_stack_, T, _foreach_callback)
#define lk_stack_foreach CONCAT3(lk_stack_, T, _foreach)

typedef struct {
    T* data;
    size_t capacity;
    size_t size;
} lk_stack;

bool lk_stack_init_with_size(lk_stack* stack, size_t count) {
    if (!stack) {
        return false;
    }
    stack->size = 0;
    stack->capacity = count;
    T* data = calloc(count, sizeof(T));
    if (!data) {
        return false;
    }
    stack->data = data;
    return true;
}

bool lk_stack_init(lk_stack* stack) {
    return lk_stack_int_init_with_size(stack, 1);
}

void lk_stack_free(lk_stack* stack) {
    if (stack) {
        free(stack->data);
        stack->data = NULL;
        stack->capacity = 0;
        stack->size = 0;
    }
}

bool lk_stack_push(lk_stack* stack, const T* const value) {
    if (!stack) {
        return false;
    }
    if (stack->size >= stack->capacity) {
        // grow using the golden ration, and add 1 in case of 1, since 1 * 1.61 doesn't *really* grow.
        stack->capacity = (size_t)((double)(stack->capacity) * 1.61) + 1;
    }
    ++stack->size;
    memcpy(&stack->data[stack->size - 1], value, sizeof(T));
    return true;
}

bool lk_stack_peek(lk_stack* stack, T* value) {
    if (!stack || !value) {
        return false;
    }
    if (stack->size > 0) {
        *value = stack->data[stack->size - 1];
        return true;
    } else {
        return false;
    }
}

enum {
    LK_BREAK,
    LK_CONTINUE,
};

typedef int (*lk_stack_foreach_callback)(T*);
// iterates over the stack from newest to oldest.
// the callback can return LK_BREAK to break, LK_CONTINUE to continue
bool lk_stack_foreach(lk_stack* stack, lk_stack_foreach_callback cb) {
    if (!stack || !cb) {
        return false;
    }
    T* iter = &stack->data[stack->size - 1];
    while (iter != &stack->data[-1]) {
        int decision = cb(iter);
        --iter;
        if (decision == LK_CONTINUE) {
            continue;
        } else if (decision == LK_BREAK) {
            break;
        } else {
            assert(!"not reachable");
        }
    }
    return true;
}
