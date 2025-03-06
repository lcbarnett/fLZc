#ifndef LZC_H
#define LZC_H

#define UNUSED __attribute__ ((unused))

#include "khash.h"

KHASH_SET_INIT_STR(str)

void dict_to_str(char* const dict, const size_t c, const char sepchar);

size_t LZc(const char* const istr, char* const dict);

void LZc_x(const char* const istr, char* const dict, size_t* const c);

#endif // LZC_H
