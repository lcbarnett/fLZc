#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <math.h>
#include <assert.h>

#include <mat.h>

#include "khashl.h"
#include "sllist.h"
#include "mt64.h"
#include "strdupt.h"

// "Diagnostic printf" macro: printf to stderr if NDEBUG not defined

#ifdef NDEBUG
#define dprintf(...)
#else
#define dprintf(...) fprintf(stderr,__VA_ARGS__)
#endif

// KHASHL string hash set and hash map initialisation (creation)

KHASHL_CSET_INIT(KH_LOCAL, strset_t, strset, const char*,         kh_hash_str, kh_eq_str)
KHASHL_CMAP_INIT(KH_LOCAL, strmap_t, strmap, const char*, size_t, kh_hash_str, kh_eq_str)

static inline double LZ76c_max(const size_t n, const int d)
{
	// single length n
	const double logd = log((double)d);
	const double logdn = log((double)n)/logd;
	return (double)n/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
}

void LZ76c_maxq(const size_t n, double* const cmax, const int d);

void LZ76c_maxv(size_t* const n, const size_t len, double* const cmax, const int d);

void tfmt(char* const tstr, const size_t tsmaxlen, const double t /* secs */);

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

// Static dictionary

static inline void ds_clear(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_clear(ddic);
}

static inline void ds_destroy(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_destroy(ddic);
}

void sd_to_str(char* const sdic, const size_t c, const char sepchar);

void sd_fprint(FILE* const fs, const char* const sdic, const size_t c, const char sepchar);

static inline void sd_print(const char* const sdic, const size_t c, const char sepchar)
{
	sd_fprint(stdout,sdic,c,sepchar);
}

mxArray* sd_to_cvec(const char* const sdic, const size_t c);

// Dynamic dictionary (hash set - cannot sort)

void ds_fprint(FILE* const fs, const strset_t* const ddic, const char sepchar);

static inline void ds_print(const strset_t* const ddic, const char sepchar)
{
	ds_fprint(stdout,ddic,sepchar);
}

mxArray* ds_to_cvec(const strset_t* const ddic);

// Dynamic dictionary (hash map - can sort)

static inline void dm_clear(strmap_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strmap_clear(ddic);
}

static inline void dm_destroy(strmap_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strmap_destroy(ddic);
}

void dm_fprint(FILE* const fs, const strmap_t* const ddic, const char sepchar);

static inline void dm_print(const strmap_t* const ddic, const char sepchar)
{
	dm_fprint(stdout,ddic,sepchar);
}

mxArray* dm_to_cvec(const strmap_t* const ddic);

// Linked-list dictionary

void dl_fprint(FILE* const fs, const ldic_t* const ldic, const char sepchar);

static inline void dl_print(const ldic_t* const ldic, const char sepchar)
{
	dl_fprint(stdout,ldic,sepchar);
}

mxArray* dl_to_cvec(const ldic_t* const ldic, const size_t c);

static inline void pputs(char* const sstart, char* const send)
{
	const char nchar = *(send+1);
	*(send+1) = 0;
	putchar('.');
	fputs(sstart,stdout);
	*(send+1) = nchar;
}

/*
static inline int substrof1(const char* const a1, const char* const a2, const char* const b1, const char* const b2)
{
	assert(a2 >= a1);
	assert(b2 >= b1);
	const size_t alen = (size_t)(a2-a1+1);
	const size_t blen = (size_t)(b2-b1+1);
	assert(blen <= alen);
	for (const char* astart = a1; astart <= a1+alen-blen; ++astart) {
		const char* a = astart;
		const char* b = b1;
		for (; b <= b2; ++a, ++b) {
			if (*b != *a) break;
		}
		if (b > b2) return 1;
	}
	return 0;
}

static inline int substrof(const char* const str, const char* const s, const char* const q)
{
	assert(s >  str);
	assert(s <= q  );
	assert(str <  s);
	for (const char* astart = str; astart < s; ++astart) {
		const char* a = astart;
		const char* b = s;
		for (; b <= q; ++a, ++b) {
			if (*b != *a) break;
		}
		if (b > q) return 1;
	}
	return 0;
}
*/

#endif // UTILS_H
