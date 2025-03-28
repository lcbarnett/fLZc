#include "LZ76c.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

size_t LZ76c(const char* const str)
{
	// LZ76c algorithm: slightly optimised version of
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.

	const size_t n = strlen(str);
	if (n == 0) return 0;
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
	// LZ76c algorithm: extended version
	//
	// This version stores LZ76c for each sequence length; c MUST be same size as the input string.

	const size_t n = strlen(str);
	if (n == 0) return;
	size_t k, w, cc = 1;
	c[0] = 1;
	if (n == 1) return;
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

size_t LZ76c_dm(char* const str, strmap_t* const ddic)
{
	// LZ76c algorithm: hash map dictionary version
	//
	// This version builds the dictionary. Note that the last word may be
	// "non-exhaustive" (see Lempel & Ziv '76), in which case it is already
	// in the dictionary and hence would not be added again; the dictionary
	// size would thus be one less than the complexity. In this function,
	// in the non-exhaustive case we "tag" the last word with a unique
	// character (which mustn't be in the alphabet!) and then add it to the
	// dictionary, so that complexity always equals  dictionary size.
	//
	// The LZc value is stored along with each word in the dictionary,so
	// that the hash map can later be sorted in word addition order.
	//
	// hash map ddic must be initialised with strmap_init(), and destroyed after use with dm_destroy()

	dm_clear(ddic);                 // clear the dictionary in case it is being re-used
	const size_t n = strlen(str);   // string length
	if (n == 0) return 0;
	int added;                      // flag for strmap_put
	size_t k, w, c = 0;             // some counters
	khint_t i = strmap_put(ddic,strndup(str,1),&added); // add first character to dictionary
	kh_val(ddic,i) = c++;           // store complexity in dictionary, and increment
	if (n == 1) return 1;           // may as well
	const char* const pend = str+n; // point past end of string
	char* p = str+1;                // next character
	while (p < pend) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {                 // last word, so p is already null-terminated
					i = strmap_put(ddic,p,&added); // check if already in dictionary if it wasn't
										           // just add it; if it was, tag and then add
dprintf("last word = [%s], %s\n",p,added?"added":"already there");
					if (added) kh_key(ddic,i) = strdup(p); else i = strmap_put(ddic,strdupt(p,TAGCHAR),&added);
					kh_val(ddic,i) = c++;          // store complexity in dictionary, and increment
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
		const char* const word = p; // word to add to dictionary
		p += (w+1);                 // end of word
		const char psave = *p;      // save char at end of word
		*p = 0;                     // NUL-terminate word
dprintf("add = [%s]\n",word);
		i = strmap_put(ddic,strdup(word),&added); // add to dictionary
		assert(added);              // word should NOT have been in dictionary!
		kh_val(ddic,i) = c++;       // store complexity in dictionary, and increment
		*p = psave;                 // restore char at end of word
	}
	assert(c == kh_size(ddic));
	return c;
}

void LZ76c_dm_x(char* const str, strmap_t* const ddic, size_t* const c)
{
	// This version stores LZ76c for each sequence length; c MUST be same size as the input string.
	//
	// hash map ddic must be initialised with strmap_init(), and destroyed after use with dm_destroy()

	dm_clear(ddic);                 // clear the dictionary in case it is being re-used
	const size_t n = strlen(str);   // string length
	if (n == 0) return;
	int added;                      // flag for strmap_put
	size_t k, w, cc = 0;            // some counters
	khint_t i = strmap_put(ddic,strndup(str,1),&added); // add first character to dictionary
	kh_val(ddic,i) = cc++;          // store complexity in dictionary, and increment	if (n == 1) {
	c[0] = 1;
	if (n == 1) return;
	const char* const pend = str+n; // point past end of string
	char* p = str+1;                // next character
	while (p < pend) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {                 // last word, so p is already null-terminated
					i = strmap_put(ddic,p,&added); // check if already in dictionary if it wasn't
										           // just add it; if it was, tag and then add
					if (added) kh_key(ddic,i) = strdup(p); else i = strmap_put(ddic,strdupt(p,TAGCHAR),&added);
					kh_val(ddic,i) = cc++;         // store complexity in dictionary, and increment
					for (const char* r = p; r < pend; ++r) c[r-str] = cc;
					assert(cc == kh_size(ddic));
					return;
				}
			}
			else {
				if (k > w) w = k;
				k = 0;
				++q;
			}
		}
		for (const char* r = p; r <= p+w; ++r) c[r-str] = cc+1;
		const char* const word = p; // word to add to dictionary
		p += (w+1);                 // end of word
		const char psave = *p;      // save char at end of word
		*p = 0;                     // NUL-terminate word
		i = strmap_put(ddic,strdup(word),&added); // add to dictionary
		assert(added);              // word should NOT have been in dictionary!
		kh_val(ddic,i) = cc++;      // store complexity in dictionary, and increment
		*p = psave;                 // restore char at end of word
	}
	assert(cc == kh_size(ddic));
}

// For testing

size_t LZ76c_ref(const char* const str)
{
	// LZ76c algorithm: reference version (testing)
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.

	const size_t n = strlen(str);
	if (n == 0) return 0;
	if (n == 1) return 1;
	size_t i = 0, k = 1, j = 1, kmax = 1, c = 1;
dprintf("%c.",str[0]);
	while (true) {
		if (str[i+k-1] == str[j+k-1]) {
			++k;
			if (j+k > n) {
				++c;
//dprintf("break 1\n");
				break;
			}
		}
		else {
			if (k > kmax) kmax = k;
			++i;
dprintf("%c",str[i+k]);
			if (i == j) {
				++c;
				j += kmax;
				if (j+1 > n) {
//dprintf("break 2\n");
					break;
				}
				i = 0;
				k = 1;
				kmax = 1;
			}
			else {
				k = 1;
			}
		}
	}
dprintf("\n");
	return c;
}

/*
int substrof(const char* const a1, const char* const a2, const char* const b1, const char* const b2)
{
	for (char*
}

size_t LZ76c_try(const char* const str)
{
	// LZ76c algorithm: reference version (testing)
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.

	if (str == 0) return 0; // empty string
	const size_t n = strlen(str);
	size_t c = 1;
	printf("%c.",str[10]);
	const char* s = str+1;
	const char* q = s+1;
	const char* p = q-1;
	while(true) {
		// is string Q: [s,q-1] a substring of the string SQpi: [str,q-2]
		if
	}
	return c;
}
*/
void LZ76c_xa(const char* const str, size_t* const c)
{
	// LZ76c algorithm: alternative extended version (testing)

	const size_t n = strlen(str);
	if (n == 0) return;
	size_t k, w, cc = 1;
	c[0] = 1;
	const char* const pend = str+n; // point past end of string
	for (const char* p = str+1; p < pend;) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {
					++cc;
					for (const char* r = p; r < pend; ++r) c[r-str] = cc;
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
		for (const char* r = p; r <= p+w; ++r) c[r-str] = cc;
		p += (w+1);
	}
}
