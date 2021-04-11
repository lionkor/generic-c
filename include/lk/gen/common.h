#ifndef COMMON_H
#define COMMON_H

typedef enum {
    LK_BREAK,
    LK_CONTINUE,
} lk_iteration_decision;

#define CONCAT_(A, B) A##B
#define CONCAT(A, B) CONCAT_(A, B)
#define CONCAT3_(A, B, C) A##B##C
#define CONCAT3(A, B, C) CONCAT3_(A, B, C)

#endif // COMMON_H
