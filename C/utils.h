#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <math.h>
#include <assert.h>

#include <mat.h>

#include "khashl.h"
#include "mt64.h"

KHASHL_CSET_INIT(KH_LOCAL, strset_t, strset, const char*, kh_hash_str, kh_eq_str)

typedef struct idic_node { // very basic single-linked list
	char* word;
	struct idic_node*     next;
} idic_t;

static inline idic_t* idic_push(idic_t* idic, char* const word)
{
	if (idic == NULL) { // first push
		idic = malloc(sizeof(idic_t));
	}
	else {
		idic->next = malloc(sizeof(idic_t));
		idic = idic->next;
	}
	idic->word = word;
	idic->next = NULL;
	return idic;
}

static inline void idic_destroy(idic_t* const idic)
{
	// burn our bridges behind us
	idic_t* pnext = idic;
	while (pnext != NULL) {
		idic_t* const p = pnext;
		pnext = p->next;
		free(p->word);
		free(p);
	}
}

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

void sdic_to_str(char* const sdic, const size_t c, const char sepchar);

void sdic_print(const char* const sdic, const size_t c, const char sepchar);

mxArray* sdic_to_cvec(const char* const sdic, const size_t c);

void ddic_print(const strset_t* const ddic, const char sepchar);

mxArray* ddic_to_cvec(const strset_t* const ddic);

void idic_print(const idic_t* const idic, const char sepchar);

mxArray* idic_to_cvec(const idic_t* const idic, const size_t c);

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

#endif // UTILS_H
