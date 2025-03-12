#include "LZc.h"
#include "mt64.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Main function

int main(int argc, char* argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Too few arguments\n");
		exit(EXIT_FAILURE);
	}

	const size_t n = (size_t)atol(argv[1]);
	const int    d = atoi(argv[2]);
	const size_t N = (size_t)atol(argv[3]);

	printf("n = %zu\n",n);
	printf("d = %d\n", d);
	printf("N = %zu\n\n",N);

	mt_t prng;        // pseudo-random number generator
	mt_seed(&prng,0); // initialise PRNG

	const double dd = (double)d;
	const double da = (double)'a';
	char* const str = malloc(n+1);
	size_t e = 0;
	str[n] = '\0'; // terminate string
	for (size_t k=0; k<N; ++k) {
		for (size_t i=0; i<n; ++i) str[i] = (char)(da+dd*mt_rand(&prng));
		const size_t cref = LZc_ref(str);
		const size_t c    = LZc(str);
		printf("%s : %3zu    %3zu",str,cref,c);
		if (c != cref) {
			printf(" ***");
			++e;
		}
		putchar('\n');
	}
	printf("\n%zu errors\n\n",e);
	free(str);

	return(EXIT_SUCCESS);
}
