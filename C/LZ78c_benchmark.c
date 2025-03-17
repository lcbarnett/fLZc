#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "LZ78c.h"
#include "mt64.h"

// Main function

int main(int argc, char* argv[])
{
	// Benchmark static vs dynamic dictionaries

	const size_t   n = argc > 1 ? (size_t)atol(argv[1])   : 10000;
	const int      d = argc > 2 ? atoi(argv[2])           : 3;
	const size_t   N = argc > 3 ? (size_t)atol(argv[3])   : 10000;
	const mtuint_t s = argc > 4 ? (mtuint_t)atol(argv[4]) : 0;

	printf("\n*** LZ78c benchmark test: static vs dynamic dictionary ***\n\n");
	printf("string length = %zu\n",n);
	printf("alphabet size = %d\n",   d);
	printf("sample size   = %zu\n",  N);
	printf("random seed   = %zu%s\n\n",s,s?"":" (random random seed :-)");

	mt_t prng; // pseudo-random number generator

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'0';

	clock_t tstart, tend;

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);
	mt_seed(&prng,s); // initialise PRNG
	printf("Starting static  run ...");
	fflush(stdout);
	tstart = clock();
	double cmeans = 0.0;
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		LZ78cs_x(str,sdic,sdlen,c);
		cmeans += (double)c[n-1];
	}
	cmeans /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeans);
	free(sdic);

	strset_t* ddic = strset_init();
	mt_seed(&prng,s); // initialise PRNG
	printf("Starting dynamic run ...");
	fflush(stdout);
	tstart = clock();
	double cmeand = 0.0;
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		LZ78cd_x(str,ddic,c);
		cmeand += (double)c[n-1];
	}
	cmeand /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeand);
	dd_destroy(ddic);

	free(c);
	free(str);

	return(EXIT_SUCCESS);
}
