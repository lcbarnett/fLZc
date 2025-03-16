#ifndef LZC_H
#define LZC_H

#define UNUSED __attribute__ ((unused))

#include "khashl.h"
#include "mt64.h"

//KHASHL_SET_INIT(KH_LOCAL, strmap_t, strmap, const char*, kh_hash_str, kh_eq_str)
KHASHL_CMAP_INIT(KH_LOCAL, strmap_t, strmap, const char*, size_t, kh_hash_str, kh_eq_str)

void dict_to_str(char* const dict, const size_t c, const char sepchar);

void dict_print(const char* const dict, const size_t c, const char sepchar);

void print_dict(const strmap_t* const h, const char sepchar, const int width);

void dict_to_c(const strmap_t* const h, size_t* const c, const size_t n);

strmap_t* LZc_build_dict(char* const str);

void LZc_destroy_dict(strmap_t* h);

size_t LZc(const char* const istr, char* const dict, const size_t dlen);

void LZc_x(const char* const istr, char* const dict, const size_t dlen, size_t* const c);

double LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev);

void LZc_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZC_H
