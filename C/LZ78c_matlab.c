#include <string.h>

#include "LZ78c_matlab.h"

mxArray* sdic_to_cvec(const char* const sdic, const size_t c)
{
	mxArray* const cvec = mxCreateCellMatrix(c,1); // should be destroyed (somewhere)
	const char* word = sdic;
	for (size_t k = 0; k < c; ++k) {
		mxSetCell(cvec,k,mxCreateString(word));
		word += (strlen(word)+1); // :-)
	}
	return cvec;
}

mxArray* ddic_to_cvec(const strset_t* const ddic)
{
	mxArray* const cvec = mxCreateCellMatrix(kh_size(ddic),1); // should be destroyed (somewhere)
	khint_t k;
	kh_foreach(ddic,k) mxSetCell(cvec,k,mxCreateString(kh_key(ddic,k)));
	return cvec;
}
/*
void LZ78c_randmv(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const cvar)
{
	// Estimate means and (optionally) variances of LZ78c for random strings with
	// with alphabet size d up to length n, based on a sample of size N, and store
	// results as a .mat file.

	mt_t prng;                     // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)seed); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'0';

	if (cvar == NULL) {
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
		for (size_t i=0; i<n; ++i) cvar[i]  = 0.0;
		for (size_t s=0; s<N; ++s) {
			for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
			str[n] = 0; // NUL-terminate string
			LZ78cs_x(str,sdic,sdlen,c);
			for (size_t i=0; i<n; ++i)  cmean[i] +=  (double)c[i];
			for (size_t i=0; i<n; ++i)  cvar[i]  += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) cvar[i] = (cvar[i]-NN*cmean[i]*cmean[i])/NN1;
	}

	free(c);
	free(sdic);
	free(str);
}
*/
