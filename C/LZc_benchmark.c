#include "LZc.h"
#include "mt64.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Main function

int main()
{
/*
	if (argc < 6) {
		fprintf(stderr, "Too few arguments\n");
		exit(EXIT_FAILURE);
	}

	const int ref = atoi(argv[1]);

	const size_t d = (size_t)atol(argv[2]);
	printf("alphabet size  = %lu\n",(unsigned long)d);

	const size_t n = (size_t)atol(argv[3]);
	printf("string length  = %lu\n",(unsigned long)n);

	const double p = atof(argv[4]);
	printf("probability    = %g\n",p);

	const size_t N = (size_t)atol(argv[5]);
	printf("sample size    = %lu\n",(unsigned long)N);

	const mtuint_t seed0 = (mtuint_t)atol(argv[6]);
	mt_t prng;                                  // pseudo-random number generator
	const mtuint_t seed = mt_seed(&prng,seed0); // initialise PRNG
	printf("random seed    = %lu\n",(unsigned long)seed);

	const double dd = (double)d;

	printf("\ninitialising strings... ");
	fflush(stdout);
	char** const s = malloc(N*sizeof(char* const));
	for (size_t r=0; r<N; ++r) {
		s[r] = malloc(n+1);
		for (size_t i=0; i<n; ++i) s[r][i] = (char)(mt_rand(&prng) < p ? 'a' : 'a'+(unsigned)floor(dd*mt_rand(&prng))   ); // 'a' with probability p
		s[r][n] = '\0';
	}
	printf("done\n\n");

//printf("\nsample = '%s'\n\n",s[0]);

	double cmean = 0.0;
	clock_t tstart, tend;

	printf("calculating complexities ... ");
	fflush(stdout);
	if (ref) {
		tstart = clock();
		for (size_t r=0; r<N; ++r) {
			cmean += (double)LZ76_ref(s[r],n);        // calculate complexity
		}
		tend = clock();
	}
	else {
		tstart = clock();
		for (size_t r=0; r<N; ++r) {
			cmean += (double)LZ76(s[r],n);        // calculate complexity
		}
		tend = clock();
	}
	printf("time (seconds) = %g\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC);
	printf("\nmean complexity = %.8f\n",cmean/(double)N);

	for (size_t r=0; r<N; ++r) free(s[r]);
	free(s);
*/
	return(EXIT_SUCCESS);
}

/*
int main(int argc, char* argv[])
{
	if (argc < 5) {
		fprintf(stderr, "Too few arguments\n");
		exit(EXIT_FAILURE);
	}

	printf("\nimplementation = %s\n",idstr);

	const size_t d = (size_t)atol(argv[1]);
	printf("alphabet size  = %lu\n",(unsigned long)d);

	const size_t n = (size_t)atol(argv[2]);
	printf("string length  = %lu\n",(unsigned long)n);

	const double p = atof(argv[3]);
	printf("probability    = %g\n",p);

	const size_t N = (size_t)atol(argv[4]);
	printf("sample size    = %lu\n",(unsigned long)N);

	const mtuint_t seed0 = (mtuint_t)atoi(argv[5]);
	mt_t prng;                                  // pseudo-random number generator
	const mtuint_t seed = mt_seed(&prng,seed0); // initialise PRNG
	printf("random seed    = %lu\n",(unsigned long)seed);

	const size_t dsize = sdlen(n,d);
	printf("dict. size     = %lu\n",(unsigned long)dsize);

	const double dd = (double)d;

	printf("\ninitialising strings... ");
	fflush(stdout);
	char** const s = malloc(N*sizeof(char* const));
	for (size_t r=0; r<N; ++r) {
		s[r] = malloc(n+1);
		for (size_t i=0; i<n; ++i) s[r][i] = (char)(mt_rand(&prng) < p ? 'a' : 'a'+(unsigned)floor(dd*mt_rand(&prng))   ); // 'a' with probability p
		s[r][n] = '\0';
	}
	printf("done\n\n");

//printf("\nsample = '%s'\n\n",s[0]);

	char* const dict = malloc(dsize);           // string dictionary
	double cmean = 0.0;
	printf("calculating complexities... ");
	fflush(stdout);
	const clock_t tstart = clock();
	for (size_t r=0; r<N; ++r) {
		memset(dict,0,dsize);                   // NUL-fill dictionary
		cmean += (double)LZc(s[r],dict);        // calculate complexity
	}
	const clock_t tend = clock();
	printf("time (seconds) = %g\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC);
	printf("\nmean complexity = %g\n",cmean/(double)N);
	free(dict);

	for (size_t r=0; r<N; ++r) free(s[r]);
	free(s);

	return(EXIT_SUCCESS);
}
*/
