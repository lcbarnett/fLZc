#include <string.h>

#include "LZ76c.h"

size_t LZ76c(const char* const str)
{
	// LZ76c
	//
	// str must be NUL-terminated.

	if (*str == 0) return 0;        // empty string
	const size_t n = strlen(str);   // length of the input string
	size_t k, w, c = 1;             // some counters
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

size_t LZ76cd(const char* const str, char* const dict)
{
	// LZ76c: returns (printable) dictionary
	//
	// str must be NUL-terminated.
	//
	// dict must have length DLEN(n), where n is the length of the input string.

	if (*str == 0) {*dict = 0; return 0;} // empty string
	const size_t n = strlen(str);         // length of the input string
	size_t k, w, c = 1;                   // some counters
	char* d = dict;
	*d++ = str[0];
	const char* pend = str+n; // point past end of string
	const char* p;
	for (p = str+1; p < pend;) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {
					*d++ = DSEPCHAR;
					for (const char* r = p; r < pend; ++r) *d++ = *r; // non-exhaustive
					*d = 0; // NUL-terminate
					return ++c;
				}
			}
			else {
				if (k > w) w = k;
				k = 0;
				++q;
			}
		}
		++c;
		*d++ = DSEPCHAR;
		for (const char* r = p+k; r <= p+w; ++r) *d++ = *r; // non-exhaustive
		p += (w+1);
	}
	if (p == pend) *d++ = DSEPCHAR; // exhaustive
	*d = 0; // NUL-terminate
	return c;
}

void LZ76cr(const char* const str, size_t* const c)
{
	// LZ76c - calculates running complexity at all sequence lengths
	//
	// str must be NUL-terminated.
	//
	// c must be same size as the input string.

	if (*str == 0) return;        // empty string
	const size_t n = strlen(str); // length of the input string
	size_t k, w,cc = 1;           // some counters
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
	return;
}

void LZ76crd(const char* const str, size_t* const c, char* const dict)
{
	// LZ76c - calculates running complexity at all sequence lengths and returns (printable) dictionary	//
	//
	// str must be NUL-terminated.
	//
	// dict must have length DLEN(n), where n is the length of the input string.
	//
	// c must be same size as the input string.

	if (*str == 0) {*dict = 0; return;} // empty string
	const size_t n = strlen(str);       // length of the input string
	size_t k, w, cc = 1;                // some counters
	c[0] = 1;
	char* d = dict;
	*d++ = str[0];
	const char* pend = str+n; // point past end of string
	const char* p;
	for (p = str+1; p < pend;) {
		k = 0;
		w = 0;
		for (const char* q = str; q < p;) {
			if (q[k] == p[k]) {
				++k;
				if (p+k >= pend) {
					++cc;
					for (const char* r = p; r < pend; ++r) c[r-str] = cc;
					*d++ = DSEPCHAR;
					for (const char* r = p; r < pend; ++r) *d++ = *r; // non-exhaustive
					*d = 0; // NUL-terminate
					return ;
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
		*d++ = DSEPCHAR;
		for (const char* r = p+k; r <= p+w; ++r) *d++ = *r; // non-exhaustive
		p += (w+1);
	}
	if (p == pend) *d++ = DSEPCHAR; // exhaustive
	*d = 0; // NUL-terminate
	return;
}

// For testing

size_t LZ76c_ref(const char* const str)
{
	// LZ76c: reference version (testing)
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.
	//
	// str must be NUL-terminated.

	if (str == 0) return 0; // empty string!
	const size_t n = strlen(str);
	if (n == 1) return 1;
	size_t i = 0, k = 1, j = 1, kmax = 1, c = 1;
	while (1) {
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
