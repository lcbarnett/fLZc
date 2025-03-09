#ifndef LZC_H
#define LZC_H

#define UNUSED __attribute__ ((unused))

#include "khash.h"
#include "mt64.h"

KHASH_SET_INIT_STR(str)

void dict_to_str(char* const dict, const size_t c, const char sepchar);

size_t LZc(const char* const istr, char* const dict, const size_t dlen);

void LZc_x(const char* const istr, char* const dict, const size_t dlen, size_t* const c);

void LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZC_H
