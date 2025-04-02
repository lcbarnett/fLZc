#include <stdlib.h>
#include <string.h>

#include "LZ78c.h"

// Main function

int test1(int argc, char* argv[])
{
	// Test dictionary

	char* const str = strdup(argc < 2 ? "0001101001000101" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string a (%lu): '%s'\n\n",n,str);

	char* const dict = malloc(DLEN(n));

	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		const size_t c = LZ78c(str,dict);
		make_printable(dict);
		printf("%6zu    %s\n",c,dict);
		str[i+1] = tmp;
	}
	putchar('\n');

	free(dict);
	free(str);

	return EXIT_SUCCESS;
}

#include "LZ76c.h"

int test2(int argc, char* argv[])
{
	// Test dictionary length

	const size_t   n = argc > 1 ? (size_t)atol(argv[1])   : 1000;
	const int      a = argc > 2 ? atoi(argv[2])           : 3;
	const char     o = argc > 3 ? argv[3][0]              : 'a';
	const size_t   N = argc > 4 ? (size_t)atol(argv[4])   : 1000;
	const mtuint_t s = argc > 5 ? (mtuint_t)atol(argv[5]) : 0;

	printf("string length   =  %zu\n",    n);
	printf("alphabet size   =  %d\n",     a);
	printf("alphabet offset = '%c'\n",    o);
	printf("sample size     =  %zu\n",    N);
	printf("random seed     =  %"PRImtui"%s\n\n",s,s?"":" (random random seed :-)");

	char* const dict76 = malloc(DLEN(n));
	char* const dict78 = malloc(DLEN(n));

	const size_t dlmax = dmaxlen(n,a);

	mt_t rng; // pseudo-random number generator
	mt_seed(&rng,s); // initialise PRNG

	char* const str  = malloc(n+1); // no need to NUL-terminate; we're going to randomise it
	size_t dl76, dl78, dl76max = 0, dl78max = 0;
	for (size_t k=0; k<N; ++k) {
		make_random_string(str,n,a,o,&rng);
		LZ76cd(str,dict76);
		LZ78c (str,dict78);
		make_printable(dict78);
		dl76 = strlen(dict76);
		if (dl76 > dl76max) dl76max = dl76;
		if (dl76 > dlmax) printf("\n*** FAIL 76\n\n");
		dl78 = strlen(dict78);
		if (dl78 > dl78max) dl78max = dl78;
		if (dl78 > dlmax) printf("\n*** FAIL 78\n\n");
		progrep("\t",k,N); fflush(stdout);
	}
	printf("*** PASS\n\n");

	printf("dictionary length (analytic)  = %zu\n",dlmax);
	printf("dictionary length (LZ76c max) = %zu\n",dl76max);
	printf("dictionary length (LZ78c max) = %zu\n\n",dl78max);

	return EXIT_SUCCESS;
}

// Main function

static const int ntests = 2;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Must specify test (1 - %d)\n",ntests);
		return(EXIT_FAILURE);
	}
	const int test = atoi(argv[1]);
	if (test < 1 || test > ntests) {
		fprintf(stderr,"Test number must be 1 - %d\n",ntests);
		return(EXIT_FAILURE);
	}

	switch (test) {
		case 1 : return test1(argc-1,argv+1);
		case 2 : return test2(argc-1,argv+1);
	}
	return(EXIT_FAILURE); // shouldn't get here!
}
