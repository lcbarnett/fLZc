#ifndef LZC76_H
#define LZC76	_H

#define UNUSED __attribute__ ((unused))

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include "utils.h"
#include "mt64.h"

size_t LZ76c_ks(const char* const str);
size_t LZ76c_wp(const char* const str);
size_t LZ76c   (const char* const str);

size_t LZ76_dict(const char* const str, strset_t* const ddic, int* const nonx);

void LZ76c_x(const char* const str, size_t* const c);

#endif // LZC76_H
