// NOTE: this will only be compiled if MATLABSTANDALONES is defined when
// running `make'. This requires that the environmental variable MATLAB_PATH
// is set to point to your matlab instllation root; see Makefile. No worries,
// it's basically for the maintainer anyway.
//
// The executable will have to be run with
//
//    LD_LIBRARY_PATH=$MATLAB_PATH/bin/glnxa64 ./LZ78c_randmv
//
// (thanks Matlab!) ... this may work on MacOS too, no clue about Windows.
//
// PS. I tried to hard-code the library path, but failed miserably; for
// reasons unknown, -Wl, rpath= ... does not appear to work.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mat.h>
#include <matrix.h>

#include "LZ78c.h"
#include "mt64.h"

// Main function

int main(int argc, char* argv[])
{
	// Estimate means and (optionally) variances of LZ78c for random strings with
	// with alphabet size d up to length n, based on a sample of size N, and store
	// results as a .mat file.

	const int         v = argc > 1 ? atoi(argv[1])           : 1;
	const size_t      n = argc > 2 ? (size_t)atol(argv[2])   : 10000;
	const int         d = argc > 3 ? atoi(argv[3])           : 3;
	const size_t      N = argc > 4 ? (size_t)atol(argv[4])   : 10000;
	const mtuint_t    s = argc > 5 ? (mtuint_t)atol(argv[5]) : 0;
	const char* const f = argc > 6 ? argv[6]                 : "/tmp/LZ78c_randmv.mat";

	printf("\ncalc. variance = %s\n",v ? "yes" : "no");
	printf("string length  = %zu\n",n);
	printf("alphabet size  = %d\n",   d);
	printf("sample size    = %zu\n",  N);
	printf("random seed    = %zu%s\n",s,s?"":" (random random seed :-)");
	printf("output file    = %s\n\n",f);

	mt_t prng;                  // pseudo-random number generator
	mt_seed(&prng,(mtuint_t)s); // initialise PRNG

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);

	size_t* const c = malloc(n*sizeof(size_t));

	double* const cmean = malloc(n*sizeof(double));
	double* const cvar  = v ? malloc(n*sizeof(double)) : NULL;

	const double dd = (double)d;
	const double da = (double)'0';

	printf("Calculating means%s ...",v ? " and variances" : ""); fflush(stdout);
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
	printf(" done\n\n");

	printf("Saving results to %s ...",f); fflush(stdout);
	MATFile* pf = matOpen(f, "w");
	if (pf == NULL) {
		fprintf(stderr," failed to open results file\n");
		return(EXIT_FAILURE);
	}

//	mxArray *pa1, *pa2, *pa3;

	if (matClose(pf) != 0) {
		fprintf(stderr,"Error closing results file\n");
		return(EXIT_FAILURE);
	}

	free(cmean);
	if (v) free(cvar);

	return EXIT_SUCCESS;
}
