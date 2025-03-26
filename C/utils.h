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

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

// Some character which definitely shoudn't be in the alphabet!
#define TAGCHAR ('*')

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

// Linked-list dictionary - a very basic single-linked list of character
// strings. Note that strings are duplicated into the list, which must be
// created with dl_create() and destroyed after use with dl_destroy().

typedef struct ldnode {
	struct ldnode* next;
	const char*    word;
} ldic_t;

static inline ldic_t* dl_create()
{
	// creates a 'dummy' node; skip when iterating
	ldic_t* const ld = malloc(sizeof(ldic_t));
	ld->word = NULL; // this is just a dummy node
	ld->next = NULL;
	return ld;
}

static inline const char* dl_word(ldic_t* ld)
{
	return ld->word;
}

static inline ldic_t* dl_push(ldic_t* ld, const char* const word)
{
	assert(ld != NULL);
	ld->next = malloc(sizeof(ldic_t));
	ld = ld->next;
	assert(word != NULL);
	ld->word = strdup(word); // this had better be null-terminated :-O
	ld->next = NULL;
	return ld;
}

static inline ldic_t* dl_push_tag(ldic_t* ld, const char* const word, const char tag)
{
	assert(ld != NULL);
	ld->next = malloc(sizeof(ldic_t));
	ld = ld->next;
	assert(word != NULL);
	ld->word = strdupt(word,tag); // this had better be null-terminated :-O
	ld->next = NULL;
	return ld;
}

static inline void dl_destroy(ldic_t* const ldic)
{
	if (ldic == NULL) return; // nothing to do
	// first we delete the dummy root node
	ldic_t* ldnext = ldic->next;
#ifndef NDEBUG
	fprintf(stderr,"freeing dummy node\n");
#endif
	free(ldic);
	// then we iterate through the other nodes, deleting as we go
	while (ldnext != NULL) {
		ldic_t* const ld = ldnext;
		ldnext = ldnext->next;
#ifndef NDEBUG
		fprintf(stderr,"freeing word [%s]\n",ld->word);
#endif
		assert(ld->word != NULL);
		free((char*)(ld->word));
		free(ld);
	}
}

void dl_print(const ldic_t* const ldic, const char sepchar);

mxArray* dl_to_cvec(const ldic_t* const ldic, const size_t c);

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

void sd_print(const char* const sdic, const size_t c, const char sepchar);

mxArray* sd_to_cvec(const char* const sdic, const size_t c);

// Dynamic dictionary (hash set - cannot sort)

void ds_print(const strset_t* const ddic, const char sepchar);

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

void dm_print(const strmap_t* const ddic, const char sepchar);

mxArray* dm_to_cvec(const strmap_t* const ddic);

#endif // UTILS_H
