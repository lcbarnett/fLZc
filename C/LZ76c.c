#include "LZ76c.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

size_t LZ76c_ks(const char* const str)
{
	// LZ76c algorithm:
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.

	const size_t n = strlen(str);
	if (n == 1) return 1;
	size_t i = 0, k = 1, j = 1, kmax = 1, c = 1;
	while (true) {
		if (str[i+k-1] == str[j+k-1]) {
			++k;
			if (j+k > n) {
				++c;
				break;
			}
		}
		else {
			if (k > kmax) kmax = k;
			++i;
			if (i == j) {
				++c;
				j += kmax;
				if (j+1 > n) break;
				i = 0;
				k = 1;
				kmax = 1;
			}
			else {
				k = 1;
			}
		}
	}
	return c;
}

size_t LZ76c_wp(const char* const str)
{
	// LZ76c algorithm:
	//
	// Wikipedia :-O

	const size_t n = strlen(str);
	size_t i = 0, c = 1, u = 1, k = 0;
	size_t kmax = k;
	while (u+k < n) {
		if (str[i+k] == str[u+k]) ++k;
		else {
			if (k > kmax) kmax = k;
			++i;
			if (i == u) { // all the pointers have been treated
				++c;
				u += (kmax+1);
				k = 0;
				i = 0;
				kmax = k;
			}
			else k = 0;
		}
	}
	if (k != 0) ++c;
	return c;
}

size_t LZ76c(const char* const str)
{
	// LZ76c algorithm:
	//
	// Somewhat optimised

	const size_t n = strlen(str);
	if (n == 1) return 1;
	size_t k, w, c = 1;
	const char* const pend = str+n; // point past end of string
	for (const char* p = str+1; p < pend;) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) return ++c;
			}
			else {
				if (k > w) w = k;
				k = 0;
				++q;
			}
		}
		++c;
		p += (w+1);
	}
	return c;
}

void LZ76c_x(const char* const str, size_t* const c)
{
	// LZ76c algorithm:
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.
	//
	// This version stores LZ76c for each sequence length; c MUST be same size as the input string.

	const size_t n = strlen(str);
	if (n == 0) return;
	size_t k, w, cc = 1;
	c[0] = 1;
	for (size_t i=1; i<n;) {
		const char* const p = str+i;
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (i+k >= n) {
					++cc;
					for (size_t j=i; j<n; ++j) c[j] = cc;
					return;
				}
			}
			else {
				if (k > w) w = k;
				k = 0;
				++q;
			}
		}
		++cc;
		for (size_t j=i; j <= i+w; ++j) c[j] = cc;
		i += (w+1);
	}
}

const char* strdupt(const char* const word, const char tag)
{
	// Duplicate word with tag character
	const size_t n = strlen(word);   // length of word
	char* const tword = malloc(n+2); // allocate extra space for the tag character
	strcpy(tword,word);              // copy last word into tagged word buffer
	tword[n] = tag;                  // insert the tag
	return tword;                    // remember to deallocate !!!
}

size_t LZ76c_d(char* const str, strset_t* const ddic)
{
	// LZ76c algorithm:
	//
	// This version builds the dictionary
	//
	// Note that the last word may be "non-exhaustive" (see Lempel & Ziv '76),
	// in which case it is already in the dictionary and hence would not be
	// added again; the dictionary size would thus be one less than the
	// complexity. In this function, in the non-exhaustive case we "tag" the
	// last word with a unique character (which mustn't be in the alphabet!)
	// and then add it to the dictionary, so that complexity always equals
	//  dictionary size.
	//
	// Remember to destroy the dictionary after use!

	dd_clear(ddic);
	const size_t n = strlen(str);
	int added;                              // flag for strset_put
	khint_t i;                              // strset_t iterator
	strset_put(ddic,strndup(str,1),&added); // add first character to dictionary
	if (n == 1) return 1;
	size_t k, w, c = 1;
	const char* const pend = str+n;         // point past end of string
	char* p = str+1;
	while (p < pend) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) { // last word
					++c;
					i = strset_put(ddic,p,&added); // check if word already in dictionary (p is last word, so already null-terminated)
					// if last word wasn't in the dictionary, just add it; else tag and then add it
					if (added) kh_key(ddic,i) = strdup(p); else strset_put(ddic,strdupt(p,TAGCHAR),&added);
					assert(c == kh_size(ddic));
					return c;
				}
			}
			else {
				if (k > w) w = k;
				k = 0;
				++q;
			}
		}
		++c;
		const char* const word = p;       // word to add to dictionary
		p += (w+1);                       // end of word
		const char psave = *p;            // save char at end of word
		*p = 0;                           // NUL-terminate word
		i = strset_put(ddic,word,&added); // check if word already in dictionary
		if (added) kh_key(ddic,i) = strdup(word); // if not, copy into dictionary
		*p = psave;                       // restore char at end of word
	}
	// if last word was in the dictionary, tag and then add it
	if (!added) strset_put(ddic,strdupt(p-(w+1),TAGCHAR),&added);
	assert(c == kh_size(ddic));
	return c;
}
