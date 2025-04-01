#include <string.h>

#include "LZ78c.h"

#include "khashl.h"

// KHASHL string hashset initialisation (creation)

KHASHL_CSET_INIT(KH_LOCAL, strset_t, strset, const char*, kh_hash_str, kh_eq_str)

// Static dictionary

size_t LZ78c(const char* const str, char* const dict)
{
	// LZ78c
	//
	// str must be NUL-terminated.
	//
	// dict must have length DLEN(n), where n is the length of the input string.

	if (*str == 0) {*dict = 0; return 0;}    // empty string
	const size_t n = strlen(str);            // length of the inpuit string
	memset(dict,0,DLEN(n));                  // NUL-fill the dictionary
	strset_t* const h = strset_init();       // create and initialise hashset
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 = '\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(h,word,&added);           // add word to hashset if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
	}
	*--w = DTRMCHAR;                         // add dictionary terminator character
	const size_t c = kh_size(h);             // LZ78c = size of dictionary
	strset_destroy(h);                       // destroy hashset
	return c;
}

void LZ78cr(const char* const str, char* const dict, size_t* const c)
{
	// LZ78c - calculates running complexity at all sequence lengths
	//
	// str must be NUL-terminated.
	//
	// dict must have length DLEN(n), where n is the length of the input string.
	//
	// c must be same size as the input string.

	if (*str == 0) {*dict = 0; return;}      // empty string
	const size_t n = strlen(str);            // length of the inpuit string
	memset(dict,0,DLEN(n));                  // NUL-fill the dictionary
	strset_t* const h = strset_init();       // create and initialise hashset
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 ='\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(h,word,&added);           // add word to hashset if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
		c[p-str] = kh_size(h);               // set current complexity to dictionary size
	}
	*--w = DTRMCHAR;                         // add dictionary terminator character
	strset_destroy(h);                       // destroy hashset
}
