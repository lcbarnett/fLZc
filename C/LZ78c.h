#ifndef LZ78C_H
#define LZ78C_H

#define UNUSED __attribute__ ((unused))

#include "khashl.h"
#include "mt64.h"

KHASHL_SET_INIT(KH_LOCAL, strmap_t, strmap, const char*, kh_hash_str, kh_eq_str)

void dict_to_str(char* const dict, const size_t c, const char sepchar);

void dict_print(const char* const dict, const size_t c, const char sepchar);

size_t LZ78c(const char* const istr, char* const dict, const size_t dlen);

void LZ78c_x(const char* const istr, char* const dict, const size_t dlen, size_t* const c);

double LZ78c_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev);

void LZ78c_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZ78C_H
