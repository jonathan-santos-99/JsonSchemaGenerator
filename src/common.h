#ifndef JGS_COMMON_H
#define JGS_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define JSG_ASSERT assert
#define private static

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define DIE() exit(1)

#define MAX(a, b) (a) > (b) ? (a) : (b)

#endif // JGS_COMMON_H