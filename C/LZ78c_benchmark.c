#include "LZ78c.h"
#include "mt64.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Main function

int main(int argc, char* argv[])
{
	const size_t n = argc > 1 ? (size_t)atol(argv[1]) : 10000;
	const int    d = argc > 2 ? atoi(argv[2])         : 3;
	const size_t N = argc > 3 ? (size_t)atol(argv[3]) : 10000;

	printf("\nn = %zu\n",n);
	printf("d = %d\n",   d);
	printf("N = %zu\n\n",N);

	mt_t prng;        // pseudo-random number generator
	mt_seed(&prng,0); // initialise PRNG

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	char* const str = malloc(n+1);
	str[n] = 0; // NUL-terminate string

	size_t* const c = malloc(n*sizeof(size_t));

	const double dd = (double)d;
	const double da = (double)'a';

	printf("Starting run...");
	fflush(stdout);
	const clock_t tstart = clock();
	double cmean = 0.0;
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		LZ78c_x(str,dict,dlen,c);
		cmean += (double)c[n-1];
	}
	cmean /= (double)N;
	const clock_t tend = clock();
	printf(" time (seconds) = %.4f\n\n",(double)(tend-tstart)/(double)CLOCKS_PER_SEC);
	printf("Mean LZ78c = %.2f\n\n",cmean);;

	free(c);
	free(str);
	free(dict);

	return(EXIT_SUCCESS);
}
