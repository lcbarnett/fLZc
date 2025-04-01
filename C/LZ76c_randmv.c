// NOTE: This is not compiled by default, as it is mostly for the benefit of
// the maintainer. See `Makefile' for build details.

#include <time.h>

#include <mat.h>

#include "LZ76c.h"

// Main function

int main(int argc, char* argv[])
{
	// Estimate means, variances and maximums of LZ76c for random strings with
	// with alphabet size a up to length n, based on a sample of size N, and store
	// results as a .mat file.

	const size_t      n = argc > 1 ? (size_t)atol(argv[1])   : 10000;
	const int         a = argc > 2 ? atoi(argv[2])           : 2;
	const size_t      N = argc > 3 ? (size_t)atol(argv[3])   : 100;
	const mtuint_t    s = argc > 4 ? (mtuint_t)atol(argv[4]) : 0;
	const char* const D = argc > 5 ? argv[5]                 : "/tmp";

	printf("\nstring length    = %zu\n",  n);
	printf("alphabet size    = %d\n",   a);
	printf("sample size      = %zu\n",  N);
	printf("random seed      = %zu%s\n",s,s?"":" (random random seed :-)");
	printf("output directory = %s\n\n",D);

	mt_t rng;                  // pseudo-random number generator
	mt_seed(&rng,(mtuint_t)s); // initialise PRNG

	char* const str  = malloc(n+1); // no need to NUL-terminate; we're going to randomise it

	size_t* const c = malloc(n*sizeof(size_t));

	mxArray* const pcmean = mxCreateDoubleMatrix(n,1,mxREAL);
	mxArray* const pcvar  = mxCreateDoubleMatrix(n,1,mxREAL);
	mxArray* const pcmax  = mxCreateDoubleMatrix(n,1,mxREAL);

	double*  const cmean = mxGetDoubles(pcmean);
	double*  const cvar  = mxGetDoubles(pcvar);
	double*  const cmax  = mxGetDoubles(pcmax);

	printf("Calculating complexities:\n\n"); fflush(stdout);
	const clock_t tstart = clock();
	for (size_t i=0; i<n; ++i) cmean[i] = 0.0;
	for (size_t i=0; i<n; ++i) cvar[i]  = 0.0;
	for (size_t i=0; i<n; ++i) cmax[i]  = 0.0;
	for (size_t s=0; s<N; ++s) {
		make_random_string(str,n,a,'0',&rng);
		LZ76cr(str,c);
		for (size_t i=0; i<n; ++i)  cmean[i] +=  (double)c[i];
		for (size_t i=0; i<n; ++i)  cvar[i]  += ((double)c[i])*((double)c[i]);
		for (size_t i=0; i<n; ++i)  cmax[i]   = c[i] > cmax[i] ? (double)c[i] : (double)cmax[i];
		progrep("\t",s,N); fflush(stdout);
	}
	const double NN = (double)N;
	for (size_t i=0; i<n; ++i) cmean[i] /= NN;
	const double NN1 = (double)(N-1);
	for (size_t i=0; i<n; ++i) cvar[i] = (cvar[i]-NN*cmean[i]*cmean[i])/NN1;
	const clock_t tend = clock();

	free(c);
	free(str);

	printf("\nCompleted in ");
	printft((double)(tend-tstart)/(double)CLOCKS_PER_SEC);
	printf("\n\n");

	const size_t sbuflen = 1000;
	char sbuf[sbuflen+1];
	snprintf(sbuf,sbuflen,"%s/LZ76c_rand_a%02d.mat",D,a);
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

	if (matPutVariable(pf,"cvar",pcvar) != 0) {
		fprintf(stderr," failed to write variable 'cvar'\n");
		return(EXIT_FAILURE);
	}
	mxDestroyArray(pcvar);

	if (matPutVariable(pf,"cmax",pcmax) != 0) {
		fprintf(stderr," failed to write variable 'cvar'\n");
		return(EXIT_FAILURE);
	}
	mxDestroyArray(pcmax);

	if (matClose(pf) != 0) {
		fprintf(stderr,"Error closing results file\n");
		return(EXIT_FAILURE);
	}
	printf(" done\n\n");

	return EXIT_SUCCESS;
}
