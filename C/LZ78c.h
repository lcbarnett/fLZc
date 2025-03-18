#ifndef LZ78C_H
#define LZ78C_H

#define UNUSED __attribute__ ((unused))

#include "utils.h"
#include "mt64.h"

// Static dictionary

size_t LZ78cs(const char* const istr, char* const dbuf, const size_t dlen);

void LZ78cs_x(const char* const istr, char* const dbuf, const size_t dlen, size_t* const c);

// Dynamic dictionary

size_t LZ78cd(char* const str, strset_t* const ddic);

void LZ78cd_x(char* const str, strset_t* const ddic, size_t* const c);

#endif // LZ78C_H
