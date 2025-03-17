#ifndef LZ78C_H
#define LZ78C_H

#define UNUSED __attribute__ ((unused))

#include "utils.h"
#include "mt64.h"

// Static dictionary

void sdic_to_str(char* const dbuf, const size_t c, const char sepchar);

void sdic_print(const char* const dbuf, const size_t c, const char sepchar);

size_t LZ78cs(const char* const istr, char* const dbuf, const size_t dlen);

void LZ78cs_x(const char* const istr, char* const dbuf, const size_t dlen, size_t* const c);

// Dynamic dictionary

void ddic_print(const strset_t* const ddic, const char sepchar);

size_t LZ78cd(char* const str, strset_t* const ddic);

void LZ78cd_x(char* const str, strset_t* const ddic, size_t* const c);

static inline void dd_clear(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_clear(ddic);
}

static inline void dd_destroy(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_destroy(ddic);
}

#endif // LZ78C_H
