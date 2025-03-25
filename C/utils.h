#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <math.h>
#include <assert.h>

#include <mat.h>

#include "khashl.h"
#include "mt64.h"

KHASHL_CSET_INIT(KH_LOCAL, strset_t, strset, const char*,         kh_hash_str, kh_eq_str)
KHASHL_MAP_INIT (KH_LOCAL,  strmap_t, strmap, const char*, size_t, kh_hash_str, kh_eq_str)

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

static inline void dds_clear(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_clear(ddic);
}

static inline void dds_destroy(strset_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strset_destroy(ddic);
}

static inline void ddm_clear(strmap_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strmap_clear(ddic);
}

static inline void ddm_destroy(strmap_t* ddic)
{
	khint_t k;
	kh_foreach(ddic,k) free((char*)kh_key(ddic,k));
	strmap_destroy(ddic);
}

// Like strdup(), but adds a "tag" character to the end of the copied string

static inline const char* strdupt(const char* const word, const char tag)
{
	// Duplicate word with tag character
	const size_t n = strlen(word);   // length of word
	char* const tword = malloc(n+2); // allocate extra space for the tag character
	strcpy(tword,word);              // copy last word into tagged word buffer
	tword[n] = tag;                  // insert the tag
	tword[n+1] = 0;                  // NUL-terminate
	return tword;                    // remember to deallocate !!!
}

// Some character which definitely shoudn't be in the alphabet!

#define TAGCHAR ('*')

void sdic_to_str(char* const sdic, const size_t c, const char sepchar);

void sdic_print(const char* const sdic, const size_t c, const char sepchar);

mxArray* sdic_to_cvec(const char* const sdic, const size_t c);

void dds_print(const strset_t* const ddic, const char sepchar);

mxArray* dds_to_cvec(const strset_t* const ddic);

void ddm_print(const strmap_t* const ddic, const char sepchar);

mxArray* ddm_to_cvec(const strmap_t* const ddic);

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

#endif // UTILS_H
