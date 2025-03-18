#include "LZ76c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

static const int ntests = 3;

int test1(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const size_t cks = LZ76c_ks(str);
	const size_t cwp = LZ76c_wp(str);
	const size_t c   = LZ76c   (str);
	printf("complexity = %zu    %zu    %zu\n\n",cks,cwp,c);

	size_t* const cc = malloc(n*sizeof(size_t));
	LZ76c_x(str,cc);
	printf("complexity =\n");
	for (size_t i=0; i<n; ++i) printf("\t%zu\n",cc[i]);
	putchar('\n');
	free(cc);

	return EXIT_SUCCESS;
}

int test2(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	size_t* const c1 = malloc(n*sizeof(size_t));
	size_t* const c2 = malloc(n*sizeof(size_t));


	LZ76c_x(str,c1);

	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		c2[i] = LZ76c(str);
		str[i+1] = tmp;
	}

	for (size_t i=0; i<n; ++i) printf("%4zu %8zu %8zu\n",i+1,c1[i], c2[i]);
	putchar('\n');

	size_t ifail = SIZE_MAX;
	for (size_t i=0; i<n; ++i) {
		if (c1[i] != c2[i]) {
			ifail = i+1;
			break;
		}
	}
	if (ifail < SIZE_MAX) {
		fprintf(stderr,"*** Failure at length %zu\n\n",ifail);
		return(EXIT_FAILURE);
	}

	free(c2);
	free(c1);
	free(str);

	return EXIT_SUCCESS;
}

int test3(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "0001101001000101" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const char sepchar = '\n';

	strset_t* ddic = strset_init();

	int nonx;
	const size_t c = LZ76_dict(str,ddic,&nonx);

	printf("LZ76c = %zu%s\n",c,nonx?" (non-exhaustive)":"");

	ddic_print(ddic,sepchar);
	putchar('\n');

	free(str);

	dd_destroy(ddic);

	return EXIT_SUCCESS;
}

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
		case 3 : return test3(argc-1,argv+1);
	}
	return(EXIT_FAILURE); // shouldn't get here!
}
