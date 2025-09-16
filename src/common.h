#ifndef JGS_COMMON_H
#define JGS_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define DA_DEFAULT_CAPACITY 8

#define DA_APPEND(da, value) {\
    if ((da).items == NULL || (da).capacity == 0) { \
        (da).items = malloc(sizeof(value) * DA_DEFAULT_CAPACITY); \
        (da).capacity = DA_DEFAULT_CAPACITY; \
        (da).count = 0; \
    } \
    if ((da).capacity <= (da).count) { \
        (da).capacity *= 2; \
        (da).items = realloc((da).items, sizeof(value)*(da).capacity); \
    } \
    (da).items[(da).count++] = value; \
} while ( 0 )

#define JSG_ASSERT assert
#define private static
#define global static

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define DIE() exit(1)

#define MAX(a, b) (a) > (b) ? (a) : (b)

#endif // JGS_COMMON_H