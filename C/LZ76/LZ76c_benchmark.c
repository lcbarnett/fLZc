#include "LZ76c.h"
#include "mt64.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Main function

int main(int argc, char* argv[])
{
	// Benchmark static vs dynamic dictionaries

	const size_t   n = argc > 1 ? (size_t)atol(argv[1])   : 10000;
	const int      d = argc > 2 ? atoi(argv[2])           : 3;
	const size_t   N = argc > 3 ? (size_t)atol(argv[3])   : 1000;
	const mtuint_t s = argc > 4 ? (mtuint_t)atol(argv[4]) : 0;

	printf("\nstring length = %zu\n",n);
	printf("alphabet size = %d\n",   d);
	printf("sample size   = %zu\n",  N);
	printf("random seed   = %zu%s\n\n",s,s?"":" (random random seed :-)");

	mt_t prng; // pseudo-random number generator

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	const double dd = (double)d;
	const double da = (double)'0';

	clock_t tstart, tend;

	mt_seed(&prng,s); // initialise PRNG
	printf("Starting reference run ...");
	fflush(stdout);
	tstart = clock();
	double cmeans = 0.0;
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		cmeans += (double)LZ76c_wp(str);
	}
	cmeans /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeans);

	mt_seed(&prng,s); // initialise PRNG
	printf("Starting optimised run ...");
	fflush(stdout);
	tstart = clock();
	double cmeand = 0.0;
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		cmeand += (double)LZ76c(str);
	}
	cmeand /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeand);

	free(str);

	return(EXIT_SUCCESS);
}
