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

size_t LZc(const char* const istr, char* const dict, const size_t dlen)
{
	// Vanilla LZc
	//
	// dict MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.

	memset(dict,0,dlen);                     // NUL-fill the dictionary
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

void LZc_x(const char* const istr, char* const dict, const size_t dlen, size_t* const c)
{
	// Vanilla LZc - return running LZc
	//
	// dict MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	memset(dict,0,dlen);                     // NUL-fill the dictionary
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

double LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZc for random strings
	// with alphabet size d of length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	double* const c = malloc(N*sizeof(double));

	const double dd = (double)d;
	for (size_t s=0; s<N; ++s) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(48+dd*mt_rand(&prng)); // 48 = '0'
		str[n] = '\0'; // terminate string
		c[s] = (double)LZc(str,dict,dlen);
	}

	double cmean = 0.0;
	for (size_t s=0; s<N; ++s) cmean += c[s];
	cmean /= (double)N;

	if (csdev != NULL) {
		*csdev = 0.0;
		for (size_t s=0; s<N; ++s) {
			const double x = c[s]-cmean;
			*csdev += x*x;
		}
		*csdev /= (double)(N-1);
	}

	free(c);
	free(dict);
	free(str);

	return cmean;
}

void LZc_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZc for random strings
	// with alphabet size d up to length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	const size_t m = N*n;
	size_t* const c = malloc(m*sizeof(size_t));

	const double dd = (double)d;
	for (size_t s=0; s<N; ++s) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(48+dd*mt_rand(&prng)); // 48 = '0'
		str[n] = '\0'; // terminate string
		LZc_x(str,dict,dlen,c+n*s);
	}

	const double dN = (double)N;
	for (size_t i=0; i<n; ++i) {
		const size_t* const ci = c+i;
		double cmi = 0.0;
		for (size_t k=0; k<m; k += n) cmi += (double)ci[k];
		cmean[i] = cmi/dN;
	}

	if (csdev != NULL) {
		const double dN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) {
			const size_t* const ci = c+i;
			const double cmi = cmean[i];
			double csi = 0.0;
			for (size_t k=0; k<m; k += n) {
				const double x = (double)ci[k]-cmi;
				csi += x*x;
			}
			csdev[i] = csi/dN1;
		}
	}

	free(c);
	free(dict);
	free(str);
}
