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
	size_t i = 0, k = 1, j = 1, K = 1, c = 1;
	while (true) {
		if (str[i+k-1] == str[j+k-1]) {
			++k;
			if (j+k > n) {
				++c;
				break;
			}
		}
		else {
			if (k > K) K = k;
			++i;
			if (i == j) {
				++c;
				j += K;
				if (j+1 > n) break;
				i = 0;
				k = 1;
				K = 1;
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
			kmax = kmax > k ? kmax : k; // vmax = max(v,vmax);
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
	size_t c = 1;
	const char* const pend = str+n; // point past end of string
	for (const char* p = str+1; p < pend;) {
		size_t k = 0;
		size_t w = 0;
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
	size_t cc = 1;
	c[0] = 1;
	for (size_t i=1; i<n;) {
		const char* const p = str+i;
		size_t k = 0;
		size_t w = 0;
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

size_t LZ76c_d(const char* const str, strset_t* const ddic, int* const nonx)
{
	// LZ76c algorithm:
	//
	// This version builds the dictionary
	//
	// Note that the last word may be "non-exhaustive" (see Lempel & Ziv '76),
	// in which case it is already in the dictionary and hence not added again.
	// In this case the dictionary size will be one less than the complexity;
	//  we set a flag `nonx' to indicate this condition..
	//
	// Remember to destroy the dictionary after use!

	dd_clear(ddic);
	const size_t n = strlen(str);
	int added;                              // flag for strset_put
	*nonx = 0;                              // non-exhaustive flag; false until we know otherwise
	strset_put(ddic,strndup(str,1),&added); // add first character to dictionary
	if (n == 1) return 1;
	size_t c = 1;
	const char* const pend = str+n;               // point past end of string
	for (const char* p = str+1; p < pend;) {
		size_t k = 0;
		size_t w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {
					++c;
					strset_put(ddic,strndup(p,(size_t)(pend-p)),&added); // add current word to dictionary if not already there
					if (!added) *nonx = 1; // last word was already in the dictionary; non-exhaustive
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
		const char* const word = p; // word to add to dictionary
		p += (w+1);
		strset_put(ddic,strndup(word,w+1),&added); // add current word to dictionary if not already there
	}
	if (!added) *nonx = 1; // last word was already in the dictionary; non-exhaustive
	return c;
}
