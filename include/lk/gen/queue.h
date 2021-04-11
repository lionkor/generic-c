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

#define lk_queue(x) CONCAT3(lk_queue_, T, x)

// see lk/gen/stack.h for an explanation on these pseudo-generics.

typedef struct {
    T* data;
    size_t capacity;
    size_t size;
    size_t readi;
    size_t writei;
} CONCAT(lk_queue_, T);

bool lk_queue(_init_with_size)(CONCAT(lk_queue_, T) * queue, size_t count) {
    if (!queue) {
        return false;
    }
    queue->capacity = count;
    queue->size = 0;
    T* data = (T*)calloc(queue->capacity, sizeof(T));
    if (!data) {
        return false;
    }
    queue->data = data;
    queue->readi = 0;
    queue->writei = 0;
    return true;
}

bool lk_queue(_init)(CONCAT(lk_queue_, T) * queue) {
    return lk_queue(_init_with_size)(queue, 5);
}

void lk_queue(_free)(CONCAT(lk_queue_, T) * queue) {
    if (queue) {
        free(queue->data);
        queue->data = NULL;
        queue->capacity = 0;
    }
}

bool lk_queue(_push)(CONCAT(lk_queue_, T) * queue, const T* const value) {
    if (!queue) {
        return false;
    }
    // TODO check capacity & size here
    if (queue->size >= queue->capacity) {
        // resize!
        queue->capacity = (size_t)((double)(queue->capacity) * 1.61);
        T* data = (T*)realloc(queue->data, queue->capacity * sizeof(T));
        if (!data) {
            return false;
        }
        queue->data = data;
        printf("resized queue to %u\n", (unsigned)queue->capacity);
    }
    queue->data[queue->writei] = *value;
    queue->writei = (queue->writei + 1) % queue->capacity;
    ++queue->size;
    return true;
}

bool lk_queue(_peek)(CONCAT(lk_queue_, T) * queue, T* value) {
    if (!queue || !value || queue->size == 0) {
        return false;
    }
    *value = queue->data[queue->readi];
    return true;
}

typedef lk_iteration_decision (*lk_queue(_foreach_callback))(T*);
// iterates over the queue from newest to oldest.
// the callback can return LK_BREAK to break, LK_CONTINUE to continue
bool lk_queue(_foreach)(CONCAT(lk_queue_, T) * queue, lk_queue(_foreach_callback) cb) {
    if (!queue || !cb) {
        return false;
    }
    return true;
}

bool lk_queue(_pop)(CONCAT(lk_queue_, T) * queue, T* out_value) {
    if (!queue || !out_value || queue->size == 0) {
        return false;
    }
    *out_value = queue->data[queue->readi];
    queue->readi = (queue->readi + 1) % queue->capacity;
    --queue->size;
    return true;
}
