#include "LZ78c.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Static dictionary

void sdic_to_str(char* const sdic, const size_t c, const char sepchar)
{
	// replace seprating NULs with seperator char

	char* word = sdic;
	for (size_t k = 0; k < c; ++k) {
		word += (strlen(word)); // :-)
		*word++ = sepchar;
	}
	*--word = 0; // NUL-terminate string
}

void sdic_print(const char* const sdic, const size_t c, const char sepchar)
{
	const char* word = sdic;
	putchar(sepchar);
	for (size_t k = 0; k < c; ++k) {
		printf("%s%c",word,sepchar);
		word += (strlen(word)+1); // :-)
	}
}

size_t LZ78cs(const char* const str, char* const sdic, const size_t sdlen)
{
	// LZ78c - static dictionary
	//
	// str MUST be NUL-terminated.
	//
	// sdic MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.

	strset_t* const dict = strset_init();    // create and initialise dictionary
	memset(sdic,0,sdlen);                    // NUL-fill the dictionary storage buffer
	char* word = sdic;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 = '\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
	}
	const size_t c = kh_size(dict);          // LZ78c = size of dictionary
	strset_destroy(dict);                    // destroy dictionary
	return c;
}

void LZ78cs_x(const char* const str, char* const sdic, const size_t sdlen, size_t* const c)
{
	// LZ78c - static dictionary
	//
	// str MUST be NUL-terminated.
	//
	// sdic MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	strset_t* const dict = strset_init();    // create and initialise dictionary
	memset(sdic,0,sdlen);                    // NUL-fill the dictionary storage buffer
	char* word = sdic;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 ='\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
		c[p-str] = kh_size(dict);            // set current complexity to dictionary size
	}
	strset_destroy(dict);                    // destroy dictionary
}

double LZ78cs_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZ78c for random strings
	// with alphabet size d of length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);

	const double dd = (double)d;
	const double da = (double)'0';

	double cmean = 0.0;
	if (csdev == NULL) {
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			cmean += (double)LZ78cs(str,sdic,sdlen);
		}
		cmean /= (double)N;
	}
	else {
		double csqrs = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			const double c = (double)LZ78cs(str,sdic,sdlen);
			cmean += c;
			csqrs += c*c;
		}
		cmean /= (double)N;
		*csdev = (csqrs-(double)N*cmean*cmean)/(double)(N-1);
	}

	free(sdic);
	free(str);

	return cmean;
}

void LZ78cs_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZ78c for random strings
	// with alphabet size d up to length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'0';

	if (csdev == NULL) {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			str[n] = 0; // NUL-terminate string
			LZ78cs_x(str,sdic,sdlen,c);
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
			LZ78cs_x(str,sdic,sdlen,c);
			for (size_t i=0; i<n; ++i)  cmean[i] += (double)c[i];
			for (size_t i=0; i<n; ++i)  csdev[i] += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) csdev[i] = (csdev[i]-NN*cmean[i]*cmean[i])/NN1;
	}

	free(c);
	free(sdic);
	free(str);
}

// Dynamic dictionary

void ddic_print(const strset_t* const ddic, const char sepchar)
{
	putchar(sepchar);
	khint_t k;
	kh_foreach(ddic,k) printf("%s%c",kh_key(ddic,k),sepchar);
}

size_t LZ78cd(char* const str, strset_t* const ddic)
{
	// LZ78c - dynamic dictionary
	//
	// str MUST be NUL-terminated.
	//
	// hash set ddic must be initialised (allocated), and must be deallocated after use!

	dd_clear(ddic);                                // clear dynamic dictionary
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strset_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash set
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			++w;                                   // extend word to next char
		}
		if (*w == 0) break;                        // finished
	}
	return kh_size(ddic);                          // return LZ78c = size of dictionary

	// remember to call dd_destroy(ddic) !!!
}

void LZ78cd_x(char* const str, strset_t* const ddic, size_t* const c)
{
	// LZ78c - dynamic dictionary
	//
	// str MUST be NUL-terminated.
	//
	// hash set ddic must be initialised (allocated) with dd_destroy(), and must be deallocated after use!
	//
	// c MUST be same size as the input string.

	dd_clear(ddic);                                // clear dynamic dictionary
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strset_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash set
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				c[w-str-1] = kh_size(ddic);        // set current complexity to dictionary size
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			c[w-str-1] = kh_size(ddic);            // set current complexity to dictionary size
			++w;                                   // extend word to next char
		}
		c[w-str-1] = kh_size(ddic);                // set current complexity to dictionary size
		if (*w == 0) break;                        // finished
	}

	// remember to call dd_destroy(ddic) !!!
}
