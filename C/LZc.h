#ifndef LZC_H
#define LZC_H

#define UNUSED __attribute__ ((unused))

#include <stddef.h>
#include <stdbool.h>

#include "mt64.h"

// Comment out as desired (WARNING: without this, needs a LOT of memory!)

size_t LZc_ref(const char* const str);

size_t LZc(const char* const str);

void LZc_x(const char* const str, size_t* const c);

double LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev);

void LZc_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZC_H
