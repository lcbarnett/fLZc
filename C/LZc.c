#include "LZc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void dict_to_str(char* const dict, const size_t c, const char sepchar)
{
	char* word = dict;
	for (size_t k = 0; k < c; ++k) {
		word += (strlen(word)); // :-)
		*word++ = sepchar;
	}
	*--word = '\0';
}

void dict_print(const char* const dict, const size_t c, const char sepchar)
{
	const char* word = dict;
	for (size_t k = 0; k < c; ++k) {
		printf("%s%c",word,sepchar);
		word += (strlen(word)+1); // :-)
	}
}

size_t LZc(const char* const istr, char* const dict)
{
	// Vanilla LZc
	//
	// dict MUST be large enough, and zero-filled (e.g., use calloc). This code does
	// not check! Safe but pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	khash_t(str)* const h = kh_init(str);    // create hash table
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* p = istr; *p; ++p) {    // traverse input string (terminating condition equiv to *p == '\0' !!!)
		*w++ = *p;                           // append next input character to word
		int added;
		kh_put(str,h,word,&added);           // add word to dictionary if not already there
		if (added) word = ++w;               // skip past '\0' and reinitialise to empty word
	}
	const size_t c = h->size;                // LZc = size of dictionary
	kh_destroy(str,h);                       // destroy hash table
	return c;
}

void LZc_x(const char* const istr, char* const dict, size_t* const c)
{
	// Vanilla LZc - return running LZc
	//
	// dict MUST be large enough, and zero-filled (e.g., use calloc). This code does
	// not check! Safe but pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	khash_t(str)* const h = kh_init(str);    // create hash table
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* p = istr; *p; ++p) {    // traverse input string (terminating condition equiv to *p == '\0' !!!)
		*w++ = *p;                           // append next input character to word
		int added;
		kh_put(str,h,word,&added);           // add word to dictionary if not already there
		if (added) word = ++w;               // skip past '\0' and reinitialise to empty word
		c[p-istr] = h->size;                 // set current complexity to dictionary size
	}
	kh_destroy(str,h);                       // destroy hash table
}
