#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lk/gen/common.h>

#ifndef T
#error "T needs to be defined"
#define T int
#endif // T

#define lk_vector(x) CONCAT3(lk_vector_, T, x)

// see lk/gen/stack.h for an explanation on these pseudo-generics.

typedef struct {
    T* data;
    size_t capacity;
    size_t size;
} CONCAT(lk_vector_, T);

bool lk_vector(_init_with_size)(CONCAT(lk_vector_, T) * vector, size_t count) {
    if (!vector) {
        return false;
    }
    vector->size = 0;
    vector->capacity = count;
    T* data = (T*)calloc(vector->capacity, sizeof(T));
    if (!data) {
        return false;
    }
    vector->data = data;
    return true;
}

bool lk_vector(_init)(CONCAT(lk_vector_, T) * vector) {
    return lk_vector(_init_with_size)(vector, 1);
}

void lk_vector(_free)(CONCAT(lk_vector_, T) * vector) {
    if (vector) {
        free(vector->data);
        vector->data = NULL;
        vector->capacity = 0;
        vector->size = 0;
    }
}

bool lk_vector(_push)(CONCAT(lk_vector_, T) * vector, const T* const value) {
    if (!vector) {
        return false;
    }
    if (vector->size >= vector->capacity) {
        // grow using the golden ration, and add 1 in case of 1, since 1 * 1.61 doesn't *really* grow.
        vector->capacity = (size_t)((double)(vector->capacity) * 1.61) + 1;
        T* data = (T*)realloc(vector->data, vector->capacity * sizeof(T));
        if (!data) {
            return false;
        }
        vector->data = data;
    }
    ++vector->size;
    vector->data[vector->size - 1] = *value;
    return true;
}

bool lk_vector(_at)(CONCAT(lk_vector_, T) * vector, size_t index, T* value) {
    if (!vector || !value || index >= vector->size) {
        return false;
    }
    if (vector->size > 0) {
        *value = vector->data[index];
        return true;
    } else {
        return false;
    }
}

typedef lk_iteration_decision (*lk_vector(_foreach_callback))(size_t, T*);
// iterates over the vector from newest to oldest.
// the callback can return LK_BREAK to break, LK_CONTINUE to continue
bool lk_vector(_foreach)(CONCAT(lk_vector_, T) * vector, lk_vector(_foreach_callback) cb) {
    if (!vector || !cb) {
        return false;
    }
    for (size_t i = 0; i < vector->size; ++i) {
        lk_iteration_decision dc = cb(i, &vector->data[i]);
        if (dc == LK_CONTINUE) {
            continue;
        } else if (dc == LK_BREAK) {
            break;
        } else {
            // not reachable
            assert(false);
        }
    }
    return true;
}

bool lk_vector(_erase)(CONCAT(lk_vector_, T) * vector, size_t i) {
    if (!vector || vector->size == 0 || i > vector->size) {
        return false;
    }
    memmove(&vector->data[i], &vector->data[i + 1], (vector->size - i - 1) * sizeof(T));
    --vector->size;
    return true;
}

typedef bool (*lk_vector(_erase_if_predicate))(const T*);
// erases all elements matching the predicate
bool lk_vector(_erase_if)(CONCAT(lk_vector_, T) * vector, lk_vector(_erase_if_predicate) cb) {
    if (!vector || !cb) {
        return false;
    }
    while (true) {
        bool erased = false;
        for (size_t i = 0; i < vector->size; ++i) {
            if (cb(&vector->data[i])) {
                lk_vector(_erase)(vector, i);
                erased = true;
                break;
            }
        }
        if (!erased) {
            break;
        }
    }
    return true;
}
