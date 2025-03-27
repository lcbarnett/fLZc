#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#include "LZ78c.h"
#include "mt64.h"

// Main function

int main(int argc, char* argv[])
{
	// Benchmark static vs dynamic dictionaries

	const size_t   n = argc > 1 ? (size_t)atol(argv[1])   : 10000;
	const int      a = argc > 2 ? atoi(argv[2])           : 3;
	const char     o = argc > 3 ? argv[3][0]              : '0';
	const size_t   N = argc > 4 ? (size_t)atol(argv[4])   : 1000;
	const mtuint_t s = argc > 5 ? (mtuint_t)atol(argv[5]) : 0;

	printf("\n*** LZ78c benchmark test: static vs dynamic dictionary ***\n\n");
	printf("string length   =  %zu\n",    n);
	printf("alphabet size   =  %d\n",     a);
	printf("alphabet offset = '%c'\n",    o);
	printf("sample size     =  %zu\n",    N);
	printf("random seed     =  %"PRImtui"%s\n\n",s,s?"":" (random random seed :-)");

	mt_t rng; // pseudo-random number generator

	char* const str  = malloc(n+1); // no need to NUL-terminate; we're going to randomise it

	size_t* const c = malloc(n*sizeof(size_t));

	clock_t tstart, tend;

//	FILE* const fs = fopen("/dev/null","w");

//	const char sepchar = '|';

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);
	mt_seed(&rng,s); // initialise PRNG
	printf("Starting static      run ...");
	fflush(stdout);
	tstart = clock();
	double cmeans = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		LZ78c_sd_x(str,sdic,sdlen,c);
//		sd_fprint(fs,sdic,c[n-1],sepchar);
		cmeans += (double)c[n-1];
	}
	cmeans /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeans);
	free(sdic);

	strset_t* const ddic = strset_init();
	mt_seed(&rng,s); // initialise PRNG
	printf("Starting dynamic     run ...");
	fflush(stdout);
	tstart = clock();
	double cmeand = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		LZ78c_ds_x(str,ddic,c);
//		ds_fprint(fs,ddic,sepchar);
		cmeand += (double)c[n-1];
	}
	cmeand /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeand);
	ds_destroy(ddic);

	ldic_t* const ldic = dl_create();
	mt_seed(&rng,s); // initialise PRNG
	printf("Starting linked-list run ...");
	fflush(stdout);
	tstart = clock();
	double cmeanl = 0.0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		LZ78c_dl_x(str,ldic,c);
//		dl_fprint(fs,ldic,sepchar);
		cmeanl += (double)c[n-1];
	}
	cmeanl /= (double)N;
	tend = clock();
	printf(" time = %.4f seconds : mean LZ78c = %.4f\n\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC,cmeanl);
	dl_destroy(ldic);

//	fclose(fs);
	free(c);
	free(str);

	return(EXIT_SUCCESS);
}
