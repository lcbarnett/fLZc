#include "LZ78c.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void dict_to_str(char* const dbuf, const size_t c, const char sepchar)
{
	// replace seprating NULs with seperator char

	char* word = dbuf;
	for (size_t k = 0; k < c; ++k) {
		word += (strlen(word)); // :-)
		*word++ = sepchar;
	}
	*--word = 0; // NUL-terminate string
}

void dict_print(const char* const dbuf, const size_t c, const char sepchar)
{
	const char* word = dbuf;
	putchar(sepchar);
	for (size_t k = 0; k < c; ++k) {
		printf("%s%c",word,sepchar);
		word += (strlen(word)+1); // :-)
	}
}

size_t LZ78c(const char* const str, char* const dbuf, const size_t dlen)
{
	// LZ78c
	//
	// str MUST be NUL-terminated.
	//
	// dbuf MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.

	strmap_t* const dict = strmap_init();    // create and initialise dictionary
	memset(dbuf,0,dlen);                     // NUL-fill the dictionary storage buffer
	char* word = dbuf;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strmap_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 = '\0' !!!)
		*w++ = *p;                           // append next input character to word
		strmap_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
	}
	const size_t c = kh_size(dict);          // LZ78c = size of dictionary
	strmap_destroy(dict);                    // destroy dictionary
	return c;
}

void LZ78c_x(const char* const str, char* const dbuf, const size_t dlen, size_t* const c)
{
	// LZ78c
	//
	// str MUST be NUL-terminated.
	//
	// dbuf MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	strmap_t* const dict = strmap_init();    // create and initialise dictionary
	memset(dbuf,0,dlen);                     // NUL-fill the dictionary storage buffer
	char* word = dbuf;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strmap_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 ='\0' !!!)
		*w++ = *p;                           // append next input character to word
		strmap_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
		c[p-str] = kh_size(dict);            // set current complexity to dictionary size
	}
	strmap_destroy(dict);                    // destroy dictionary
}

double LZ78c_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZ78c for random strings
	// with alphabet size d of length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t dlen = 2*n;
	char* const dbuf = malloc(dlen);

	const double dd = (double)d;
	const double da = (double)'a';

	double cmean = 0.0;
	if (csdev == NULL) {
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			cmean += (double)LZ78c(str,dbuf,dlen);
		}
		cmean /= (double)N;
	}
	else {
		double csqrs = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			const double c = (double)LZ78c(str,dbuf,dlen);
			cmean += c;
			csqrs += c*c;
		}
		cmean /= (double)N;
		*csdev = (csqrs-(double)N*cmean*cmean)/(double)(N-1);
	}

	free(dbuf);
	free(str);

	return cmean;
}

void LZ78c_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZ78c for random strings
	// with alphabet size d up to length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t dlen = 2*n;
	char* const dbuf = malloc(dlen);

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'a';

	if (csdev == NULL) {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			str[n] = 0; // NUL-terminate string
			LZ78c_x(str,dbuf,dlen,c);
			for (size_t i=0; i<n; ++i) cmean[i] += (double)c[i];
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
	}
	else {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t i=0; i<n; ++i) csdev[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			str[n] = 0; // NUL-terminate string
			LZ78c_x(str,dbuf,dlen,c);
			for (size_t i=0; i<n; ++i)  cmean[i] += (double)c[i];
			for (size_t i=0; i<n; ++i)  csdev[i] += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) csdev[i] = (csdev[i]-NN*cmean[i]*cmean[i])/NN1;
	}

	free(c);
	free(dbuf);
	free(str);
}
