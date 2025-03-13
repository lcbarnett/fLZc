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
	putchar(sepchar);
	for (size_t k = 0; k < c; ++k) {
		printf("%s%c",word,sepchar);
		word += (strlen(word)+1); // :-)
	}
}

void print_dict(const strmap_t* const h, const char sepchar, const int width)
{
	khint_t k;
	if (width == 0) {
		putchar(sepchar);
		kh_foreach(h,k) printf("%s%c",(char*)kh_key(h,k),sepchar);
	}
	else {
		const size_t fmtmax = 21;
		char fmtstr[fmtmax];
		snprintf(fmtstr,fmtmax,"%%%dzu  %%s%%c",width);
		putchar(sepchar);
		kh_foreach(h,k) printf(fmtstr,kh_val(h,k),(char*)kh_key(h,k),sepchar);
	}
}

void dict_to_c(const strmap_t* const h, size_t* const c, const size_t n)
{
	for (size_t i=0; i<n; ++i) c[i] = 0;
	khint_t k;
	kh_foreach(h,k) for (size_t i = kh_val(h,k)+1; i<n; ++i) ++(c[i]);
}

strmap_t* LZc_build_dict(char* const str)
{
	strmap_t* const h = strmap_init(); // create and initialise hash set
	int added;
	char qchar;
	char* p = str;
	while (1) {
		char* q = p+1;
		while (1) {
			qchar = *q;
			*q = '\0';
			khint_t k = strmap_put(h,p,&added);
			if (added) {
				kh_key(h,k) = strdup(p);
				kh_val(h,k) = (size_t)(q-str);
				p = q;
				*q = qchar;
				break;
			}
			*q = qchar;
			if (*q == 0) break;
			++q;
		}
		if (*q == 0) break;
	}

	return h; // return pointer to hash set
}

void LZc_destroy_dict(strmap_t* h)
{
	khint_t k;
	kh_foreach(h,k) free((char*)kh_key(h,k));
	strmap_destroy(h);
}

size_t LZc(const char* const str, char* const dict, const size_t dlen)
{
	// LZ78c
	//
	// dict MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.

	strmap_t* const h = strmap_init();       // create and initialise hash set
	memset(dict,0,dlen);                     // NUL-fill the dictionary
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == '\0' !!!)
		*w++ = *p;                           // append next input character to word
		int added;
		strmap_put(h,word,&added);           // add word to dictionary if not already there
		if (added) {
//fprintf(stderr,"added [%s]\n",word);
			word = ++w;               // skip past '\0' and reinitialise to empty word
		}
	}
	const size_t c = kh_size(h);             // LZc = size of dictionary
	strmap_destroy(h);                       // destroy hash set
	return c;
}

void LZc_x(const char* const str, char* const dict, const size_t dlen, size_t* const c)
{
	// LZ78c
	//
	// dict MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	strmap_t* const h = strmap_init();       // create and initialise hash set
	memset(dict,0,dlen);                     // NUL-fill the dictionary
	char* word = dict;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == '\0' !!!)
		*w++ = *p;                           // append next input character to word
		int added;
		strmap_put(h,word,&added);           // add word to dictionary if not already there
		if (added) word = ++w;               // skip past '\0' and reinitialise to empty word
		c[p-str] = kh_size(h);               // set current complexity to dictionary size
	}
	strmap_destroy(h);                       // destroy hash set
}

double LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZc for random strings
	// with alphabet size d of length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = '\0'; // terminate string

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	const double dd = (double)d;
	const double da = (double)'a';

	double cmean = 0.0;
	if (csdev == NULL) {
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			cmean += (double)LZc(str,dict,dlen);
		}
		cmean /= (double)N;
	}
	else {
		double csqrs = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			const double c = (double)LZc(str,dict,dlen);
			cmean += c;
			csqrs += c*c;
		}
		cmean /= (double)N;
		*csdev = (csqrs-(double)N*cmean*cmean)/(double)(N-1);
	}

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
	str[n] = '\0'; // terminate string

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'a';

	if (csdev == NULL) {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			str[n] = '\0'; // terminate string
			LZc_x(str,dict,dlen,c);
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
			str[n] = '\0'; // terminate string
			LZc_x(str,dict,dlen,c);
			for (size_t i=0; i<n; ++i)  cmean[i] += (double)c[i];
			for (size_t i=0; i<n; ++i)  csdev[i] += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) csdev[i] = (csdev[i]-NN*cmean[i]*cmean[i])/NN1;
	}

	free(c);
	free(dict);
	free(str);
}
