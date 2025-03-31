#include <stdlib.h>
#include <time.h>

#include "LZ76c.h"

// Main function

int main(int argc, char* argv[])
{
	// Benchmark reference vs optimised algorithm

	const size_t   n = argc > 1 ? (size_t)atol(argv[1])   : 4000;
	const int      a = argc > 2 ? atoi(argv[2])           : 3;
	const char     o = argc > 3 ? argv[3][0]              : '0';
	const size_t   N = argc > 4 ? (size_t)atol(argv[4])   : 1000;
	const mtuint_t s = argc > 5 ? (mtuint_t)atol(argv[5]) : 912710183171;

	printf("\n*** LZ76c benchmark test: reference vs optimised algorithm ***\n\n");
	printf("string length   =  %zu\n",    n);
	printf("alphabet size   =  %d\n",     a);
	printf("alphabet offset = '%c'\n",    o);
	printf("sample size     =  %zu\n",    N);
	printf("random seed     =  %"PRImtui"%s\n\n",s,s?"":" (random random seed :-)");

	mt_t rng; // pseudo-random number generator

	char* const str  = malloc(n+1); // no need to NUL-terminate; we're going to randomise it

	clock_t tstart, tend;

	mt_seed(&rng,s); // initialise PRNG
	printf("Starting dummy     run ...");
	fflush(stdout);
	tstart = clock();
	double cmeanx = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		cmeanx += (double)LZ76c_ref(str);
	}
	cmeanx /= (double)N;
	tend = clock();
	printf(" %8.4f seconds : mean LZ76c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeanx);

	mt_seed(&rng,s); // initialise PRNG
	printf("Starting reference run ...");
	fflush(stdout);
	tstart = clock();
	double cmeans = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		cmeans += (double)LZ76c_ref(str);
	}
	cmeans /= (double)N;
	tend = clock();
	printf(" %8.4f seconds : mean LZ76c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeans);

	mt_seed(&rng,s); // initialise PRNG
	printf("Starting optimised run ...");
	fflush(stdout);
	tstart = clock();
	double cmeand = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		cmeand += (double)LZ76c(str);
	}
	cmeand /= (double)N;
	tend = clock();
	printf(" %8.4f seconds : mean LZ76c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeand);

	free(str);

	return(EXIT_SUCCESS);
}
