// NOTE: This is not compiled by default, as it is mostly for the benefit of
// the maintainer. See `Makefile' for build details.

#include <time.h>

#include <mat.h>

#include "LZ78c.h"

// Main function

int main(int argc, char* argv[])
{
	// Estimate means and (optionally) variances of LZ78c for random strings with
	// with alphabet size d up to length n, based on a sample of size N, and store
	// results as a .mat file.
	//
	// We use dict rather than ddic, as it is somewhat more efficient in this scenario.

	const int         v = argc > 1 ? atoi(argv[1])           : 1;
	const size_t      n = argc > 2 ? (size_t)atol(argv[2])   : 10000;
	const int         a = argc > 3 ? atoi(argv[3])           : 2;
	const size_t      N = argc > 4 ? (size_t)atol(argv[4])   : 10000;
	const mtuint_t    s = argc > 5 ? (mtuint_t)atol(argv[5]) : 0;
	const char* const D = argc > 6 ? argv[6]                 : "/tmp";

	printf("\ncalc. variance   = %s\n",v ? "yes" : "no");
	printf("string length    = %zu\n",  n);
	printf("alphabet size    = %d\n",   a);
	printf("sample size      = %zu\n",  N);
	printf("random seed      = %zu%s\n",s,s?"":" (random random seed :-)");
	printf("output directory = %s\n\n",D);

	mt_t rng;                  // pseudo-random number generator
	mt_seed(&rng,(mtuint_t)s); // initialise PRNG

	char* const str  = malloc(n+1); // no need to NUL-terminate; we're going to randomise it

	size_t* const c = malloc(n*sizeof(size_t));

	char* const dict = malloc(DLEN(n));

	mxArray* const pcmean = mxCreateDoubleMatrix(n,1,mxREAL);
	double*  const cmean  = mxGetDoubles(pcmean);

	mxArray* const pcvar = v ? mxCreateDoubleMatrix(n,1,mxREAL) : NULL;
	double*  const cvar  = v ? mxGetDoubles(pcvar) : NULL;

	printf("Calculating means%s ...",v ? " and variances" : ""); fflush(stdout);
	const clock_t tstart = clock();
	if (cvar == NULL) {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t s=0; s<N; ++s) {
			make_random_string(str,n,a,'0',&rng);
			LZ78cr(str,dict,c);
			for (size_t i=0; i<n; ++i) cmean[i] += (double)c[i];
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
	}
	else {
		for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
		for (size_t i=0; i<n; ++i) cvar[i]  = 0.0;
		for (size_t s=0; s<N; ++s) {
			make_random_string(str,n,a,'0',&rng);
			LZ78cr(str,dict,c);
			for (size_t i=0; i<n; ++i)  cmean[i] +=  (double)c[i];
			for (size_t i=0; i<n; ++i)  cvar[i]  += ((double)c[i])*((double)c[i]);
		}
		const double NN = (double)N;
		for (size_t i=0; i<n; ++i) cmean[i] /= NN;
		const double NN1 = (double)(N-1);
		for (size_t i=0; i<n; ++i) cvar[i] = (cvar[i]-NN*cmean[i]*cmean[i])/NN1;
	}
	const clock_t tend = clock();
	free(c);
	free(dict);
	free(str);

	const size_t sbuflen = 1000;
	char sbuf[sbuflen+1];

	tfmt(sbuf,sbuflen,(double)(tend-tstart)/(double)CLOCKS_PER_SEC);
	printf(" done in %s\n\n",sbuf);

	snprintf(sbuf,sbuflen,"%s/LZ78c_rand_a%02d.mat",D,a);
	printf("Saving results to %s ...",sbuf); fflush(stdout);
	MATFile* const pf = matOpen(sbuf, "w");
	if (pf == NULL) {
		fprintf(stderr," failed to open results file\n");
		return(EXIT_FAILURE);
	}

	mxArray* p;
	p = mxCreateDoubleScalar((double)n);
	if (matPutVariable(pf,"nmax",p) != 0) {
		fprintf(stderr," failed to write variable 'n'\n");
		return(EXIT_FAILURE);
	}
	p = mxCreateDoubleScalar((double)a);
	if (matPutVariable(pf,"asize",p) != 0) {
		fprintf(stderr," failed to write variable 'a'\n");
		return(EXIT_FAILURE);
	}
	p = mxCreateDoubleScalar((double)N);
	if (matPutVariable(pf,"nsamples",p) != 0) {
		fprintf(stderr," failed to write variable 'N'\n");
		return(EXIT_FAILURE);
	}
	p = mxCreateDoubleScalar((double)s);
	if (matPutVariable(pf,"rseed",p) != 0) {
		fprintf(stderr," failed to write variable 's'\n");
		return(EXIT_FAILURE);
	}
	mxDestroyArray(p);

	if (matPutVariable(pf,"cmean",pcmean) != 0) {
		fprintf(stderr," failed to write variable 'cmean'\n");
		return(EXIT_FAILURE);
	}
	mxDestroyArray(pcmean);

	if (v) {
		if (matPutVariable(pf,"cvar",pcvar) != 0) {
			fprintf(stderr," failed to write variable 'cvar'\n");
			return(EXIT_FAILURE);
		}
		mxDestroyArray(pcvar);
	}

	if (matClose(pf) != 0) {
		fprintf(stderr,"Error closing results file\n");
		return(EXIT_FAILURE);
	}
	printf(" done\n\n");

	return EXIT_SUCCESS;
}
