#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <lk/gen/common.h>

#ifndef T
#error "T needs to be defined"
#define T int
#endif // T

#define lk_stack(x) CONCAT3(lk_stack_, T, x)

/*
 * About reading pseudo-generic code:
 * - lk_stack(_free) is to be read as lk_stack_T_free, so lk_stack_free for this T.
 *   if T is int, this would be lk_stack_int_free
 * - CONCAT(lk_stack_, T) is to be read as lk_stack_T, so with T = int lk_stack_int.
 *
 * When calling these, you call them by their resulting name; lk_stack(_free) on an int
 * stack is to be called like: lk_stack_int_free
 */

typedef struct {
    T* data;
    size_t capacity;
    size_t size;
} CONCAT(lk_stack_, T);

bool lk_stack(_init_with_size)(CONCAT(lk_stack_, T) * stack, size_t count) {
    if (!stack) {
        return false;
    }
    stack->size = 0;
    stack->capacity = count;
    T* data = calloc(stack->capacity, sizeof(T));
    if (!data) {
        return false;
    }
    stack->data = data;
    return true;
}

bool lk_stack(_init)(CONCAT(lk_stack_, T) * stack) {
    return lk_stack(_init_with_size)(stack, 1);
}

void lk_stack(_free)(CONCAT(lk_stack_, T) * stack) {
    if (stack) {
        free(stack->data);
        stack->data = NULL;
        stack->capacity = 0;
        stack->size = 0;
    }
}

bool lk_stack(_push)(CONCAT(lk_stack_, T) * stack, const T* const value) {
    if (!stack) {
        return false;
    }
    if (stack->size >= stack->capacity) {
        // grow using the golden ration, and add 1 in case of 1, since 1 * 1.61 doesn't *really* grow.
        stack->capacity = (size_t)((double)(stack->capacity) * 1.61) + 1;
        T* data = realloc(stack->data, stack->capacity * sizeof(T));
        if (!data) {
            return false;
        }
        stack->data = data;
    }
    ++stack->size;
    stack->data[stack->size - 1] = *value;
    return true;
}

bool lk_stack(_peek)(CONCAT(lk_stack_, T) * stack, T* value) {
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

typedef lk_iteration_decision (*lk_stack(_foreach_callback))(T*);
// iterates over the stack from newest to oldest.
// the callback can return LK_BREAK to break, LK_CONTINUE to continue
bool lk_stack(_foreach)(CONCAT(lk_stack_, T) * stack, lk_stack(_foreach_callback) cb) {
    if (!stack || !cb) {
        return false;
    }
    T* iter = &stack->data[stack->size - 1];
    while (iter != &stack->data[-1]) {
        lk_iteration_decision decision = cb(iter);
        --iter;
        if (decision == LK_CONTINUE) {
            continue;
        } else if (decision == LK_BREAK) {
            break;
        } else {
            // not reachable
            assert(false);
        }
    }
    return true;
}

bool lk_stack(_pop)(CONCAT(lk_stack_, T) * stack, T* out_value) {
    if (!stack || !out_value) {
        return false;
    }
    if (stack->size == 0) {
        return false;
    }
    *out_value = stack->data[stack->size - 1];
    --stack->size;
    return true;
}
