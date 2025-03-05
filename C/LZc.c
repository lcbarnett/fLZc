#include "LZc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

size_t maxwordlen(const size_t n)
{
	return (size_t)floor((sqrt((double)(8*n+1))-1.0)/2.0); // worst-case scenario
}

size_t sdlen(const size_t n, const size_t d)
{
	return (d == 0 ? n : cmax(n,d))+n+1; // max string dictionary size
}

size_t cmax(const size_t n,const size_t d)
{
	size_t g  = 0; // g_k(d)     = d + 2*d^2 + ... + k*d^k
	size_t f  = 0; // f_{k-1}(d) = d + d^2 + ... + d^{k-1}
	size_t dk = 1; // d^k
	size_t k  = 1;
	size_t n0;
	while (1) {
		dk *= d;
		n0  = g;          // the D = k' = 0 string length
		g  += k*dk;
		if (g > n) break; // overshot (note k has also overshot!)
		f += dk;
		++k;
	}
	return f + (size_t)floor((double)(n-n0)/(double)k); // f_{k-1}(d) + D, D = floor((n-g_{k-1}(d))/k)
	//k = k-1; % overshot, so decrement k
}

void cmax_x(size_t* const c, const size_t nmax, const size_t d)
{
	for (size_t n=1; n<=d; ++n) {
		if (n > nmax) return;
		c[n-1] = n;
	}
	size_t g  = 0;
	size_t f  = 0;
	size_t dk = 1;
	size_t k  = 1;
	while (1) {
		dk *= d;
		f  += dk;
		g  += k*dk;
		++k;
		for (size_t D=0; D<d*dk; ++D) {
			const size_t cn = f+D;
			for (size_t n=g+k*D; n<=g+k*(D+1); ++n) {
				if (n > nmax) return;
				c[n-1] = cn;
			}
		}
	}
}

void dict_to_str(char* const dict)
{
	char* word = dict;
	while (*word != DTRMCHAR) {
		word += (strlen(word)); // :-)
		*word++ = DSEPCHAR;
	}
	*--word = '\0'; // we don't want the final separator (since there is no initial one...)
}

void dict_print(const char* const dict)
{
	const char* word = dict;
	printf("%c",DSEPCHAR);
	while (*word != DTRMCHAR) {
		printf("%s%c",word,DSEPCHAR);
		word += (strlen(word)+1); // :-)
	}
}

#ifdef GLIBVER

size_t LZc(const char* const istr, char* const dict)
{
	// dict MUST be large enough, and NUL-filled! Pessimistic case is
	// length of istr + cmax + 1 (for the NUL-terminators and dictionary
	// terminator). Even more pessimistic (but doesn't need to know the
	// dictionary size) is twice the string length + 1.

	GHashTable* const h = g_hash_table_new(g_str_hash,g_str_equal); // create hash table
	size_t c = 0;                                  // dictionary size
	char* word = dict;                             // initialise pointer to beginning of current word
	char* w = word;                                // pointer to end of current word (null terminator)
	for (const char* ch = istr; *ch; ++ch) {       // traverse input string (terminating condition equiv to *ch == '\0' !!!)
		*w++ = *ch;                                // append next input character to word
		if (g_hash_table_lookup(h,word) == NULL) { // if word not in dictionary,
			g_hash_table_add(h,word);              // add word to dictionary (treated as a hash set!)
			word = ++w;                            // skip past NUL and reinitialise to empty word
			++c;                                   // increment dictionary size
		}
	}
	*word = DTRMCHAR;                              // terminate the dictionary (DTRMCHAR must NOT be '\0' !!!)
	g_hash_table_destroy(h);                       // destroy hash table
	return c;
}

void LZc_x(const char* const istr, char* const dict, size_t* const c)
{
	// dict MUST be large enough, and NUL-filled! Pessimistic case is
	// length of istr + cmax + 1 (for the NUL-terminators and dictionary
	// terminator). Even more pessimistic (but doesn't need to know the
	// dictionary size) is twice the string length + 1.
	//
	// c MUST be same size as the input string.

	GHashTable* const h = g_hash_table_new(g_str_hash,g_str_equal); // create hash table
	size_t cc = 0;                                 // dictionary size
	char* word = dict;                             // initialise pointer to beginning of current word
	char* w = word;                                // pointer to end of current word (null terminator)
	for (const char* ch = istr; *ch; ++ch) {       // traverse input string (terminating condition equiv to *ch == '\0' !!!)
		*w++ = *ch;                                // append next input character to word
		if (g_hash_table_lookup(h,word) == NULL) { // if word not in dictionary,
			g_hash_table_add(h,word);              // add word to dictionary (treated as a hash set!)
			word = ++w;                            // skip past NUL and reinitialise to empty word
			++cc;                                  // increment dictionary size
		}
		c[ch-istr] = cc;                           // set current complexity to dictionary size
	}
	*word = DTRMCHAR;                              // terminate the dictionary (DTRMCHAR must NOT be '\0' !!!)
	g_hash_table_destroy(h);                       // destroy hash table
}

#else

size_t LZc(const char* const istr, char* const dict)
{
	// dict MUST be large enough, and NUL-filled! Pessimistic case is
	// length of istr + cmax + 1 (for the NUL-terminators and dictionary
	// terminator). Even more pessimistic (but doesn't need to know the
	// dictionary size) is twice the string length + 1.

	khash_t(str)* const h = kh_init(str);    // create hash table
	size_t c = 0;                            // dictionary size
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* ch = istr; *ch; ++ch) { // traverse input string (terminating condition equiv to *ch == '\0' !!!)
		*w++ = *ch;                          // append next input character to word
		int absent;
		kh_put(str,h,word,&absent);          // if word not in dictionary,
		if (absent) {
			word = ++w;                      // skip past NUL and reinitialise to empty word
			++c;                             // increment dictionary size
		}
	}
	*word = DTRMCHAR;                        // terminate the dictionary (DTRMCHAR must NOT be '\0' !!!)
	kh_destroy(str,h);                       // destroy hash table
	return c;
}

void LZc_x(const char* const istr, char* const dict, size_t* const c)
{
	// dict MUST be large enough, and NUL-filled! Pessimistic case is
	// length of istr + cmax + 1 (for the NUL-terminators and dictionary
	// terminator). Even more pessimistic (but doesn't need to know the
	// dictionary size) is twice the string length + 1.
	//
	// c MUST be same size as the input string.

	khash_t(str)* const h = kh_init(str);    // create hash table
	size_t cc = 0;                           // dictionary size
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* ch = istr; *ch; ++ch) { // traverse input string (terminating condition equiv to *ch == '\0' !!!)
		*w++ = *ch;                          // append next input character to word
		int absent;
		kh_put(str,h,word,&absent);          // if word not in dictionary,
		if (absent) {
			word = ++w;                      // skip past NUL and reinitialise to empty word
			++cc;                            // increment dictionary size
		}
		c[ch-istr] = cc;                     // set current complexity to dictionary size
	}
	*word = DTRMCHAR;                        // terminate the dictionary (DTRMCHAR must NOT be '\0' !!!)
	kh_destroy(str,h);                       // destroy hash table
}

#endif // GLIBVER

size_t LZ76_ref(const char* const istr, const size_t n)
{
	// Computes the LZ76 complexity index of a character sequence using the
	// algorithm described in F. Kaspar, H. G. Schuster, "Easily-calculable
	// measure for the complexity of spatiotemporal patterns", Physical Review
	// A, Volume 36, Number 2 (1987).

#ifdef DEBUG
if (n > 0) printf("m =  1, c = 1 : %c\n",istr[0]);
#endif
	if (n < 2) return n;
	size_t c = 1;
	size_t m = 1;
	while (1) {
		size_t i = 0;
		size_t k = 1;
		size_t w = 1;
		while (1) {
			if (istr[i+k-1] == istr[m+k-1]) {
				++k;
				if (m+k > n) return ++c;
			}
			else {
				if (k > w) w = k;
				if (++i == m) {
					++c;
#ifdef DEBUG
printf("m = %2lu, c = %lu : ",m+1,c); fwrite(istr+m,1,w,stdout); printf("\n");
#endif
					m += w;
					if (m+1 > n) return ++c;
					break;
				}
				k = 1;
			}
		}
	}
}

size_t LZ76(const char* const istr, const size_t n)
{
	// Computes the LZ76 complexity index of a character sequence using the
	// algorithm described in F. Kaspar, H. G. Schuster, "Easily-calculable
	// measure for the complexity of spatiotemporal patterns", Physical Review
	// A, Volume 36, Number 2 (1987).

#ifdef DEBUG
if (n > 0) printf("m =  1, c = 1 : %c\n",istr[0]);
#endif
	if (n < 2) return n;
	size_t c = 1;
	const char* const pend = istr+n; // point past end of string
	for (const char* p = istr+1; p < pend;) {
		size_t k = 0;
		size_t w = 0;
		for (const char* q = istr; q < p;) {
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
#ifdef DEBUG
printf("m = %2lu, c = %lu : ",p-istr+1,c); fwrite(p,1,w+1,stdout); printf("\n");
#endif
		p += (w+1);
	}
	return c;
}

void LZ76_x(const char* const istr, const size_t n, size_t* const c)
{
	// Computes the LZ76 complexity index of a character sequence using the
	// algorithm described in F. Kaspar, H. G. Schuster, "Easily-calculable
	// measure for the complexity of spatiotemporal patterns", Physical Review
	// A, Volume 36, Number 2 (1987).

	if (n == 0) return;
	size_t cc = 1;
#ifdef DEBUG
printf("m =  1, c = 1 : %c\n",istr[0]);
#endif
	c[0] = 1;
	for (size_t i=1; i<n;) {
		const char* const p = istr+i;
		size_t k = 0;
		size_t w = 0;
		for (const char* q = istr; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (i+k >= n) {
					++cc;
					for (size_t j=i; j < n; ++j) c[j] = cc;
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
#ifdef DEBUG
printf("m = %2lu, c = %lu : ",i+1,cc); fwrite(istr+i,1,w+1,stdout); printf("\n");
#endif
		for (size_t j=i; j <= i+w; ++j) c[j] = cc;
		i += (w+1);
	}
}
