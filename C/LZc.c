#include "LZc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

size_t LZc_ref(const char* const str)
{
	// LZc algorithm:
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

size_t LZc(const char* const str)
{
	// LZc algorithm:
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.

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

void LZc_x(const char* const str, size_t* const c)
{
	// LZc algorithm:
	//
	// F. Kaspar and H. G. Schuster, "Easily calculable measure for the complexity of spatiotemporal patterns",
	// Phys. Rev. A 36(2) pp. 842-848, 1987.
	//
	// This version stores LZc for each sequence length; cc MUST be same size as the input string.

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

double LZc_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev)
{
	// Estimate means and (optionally) standard deviations of LZc for random strings
	// with alphabet size d of length n, based on a sample of size N.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = '\0'; // terminate string

	const double dd = (double)d;
	const double da = (double)'a';

	double cmean = 0.0;
	if (csdev == NULL) {
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			cmean += (double)LZc(str);
		}
		cmean /= (double)N;
	}
	else {
		double csqrs = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)('a'+dd*mt_rand(&prng));
			str[n] = '\0'; // terminate string
			const double c = (double)LZc(str);
			cmean += c;
			csqrs += c*c;
		}
		cmean /= (double)N;
		*csdev = (csqrs-(double)N*cmean*cmean)/(double)(N-1);
	}

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

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'a';

	if (csdev == NULL) {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			LZc_x(str,c);
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
			LZc_x(str,c);
			for (size_t i=0; i<n; ++i)  cmean[i] += (double)c[i];
			for (size_t i=0; i<n; ++i)  csdev[i] += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) csdev[i] = (csdev[i]-NN*cmean[i]*cmean[i])/NN1;
	}

	free(c);
	free(str);
}
