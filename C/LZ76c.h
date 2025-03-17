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

void LZ76c_x(const char* const str, size_t* const c);

double LZ76c_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev);

void LZ76c_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZC76_H
