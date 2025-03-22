#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LZ76c.h"
#include "mt64.h"

// Tests

int test1(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const size_t cref = LZ76c_ref(str);
	const size_t c    = LZ76c    (str);
	printf("complexity = %zu    %zu\n\n",cref,c);

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
	char* const  str = strdup(argc < 2 ? "00010100001010010010111110001011" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	strset_t* ddic = strset_init();

	for (size_t k = 0; k < n; ++k) {
		str[n-k] = 0;
		const size_t c = LZ76c_d(str,ddic);
		printf("LZ76c = %3zu : ",c);
		ddic_print(ddic,'|');
		putchar('\n');
	}
	putchar('\n');

	free(str);

	dd_destroy(ddic);

	return EXIT_SUCCESS;
}

int test4(int UNUSED argc, char UNUSED * argv[])
{
	idic_t* p = idic_push(NULL,strdup("Hello!"));
	idic_t* const idic = p; // bottom of the dictionary
	p = idic_push(p,strdup("How"));
	p = idic_push(p,strdup("are"));
	p = idic_push(p,strdup("you?"));

	putchar('\n');
	idic_print(idic,'|');
	putchar('\n');
	putchar('\n');

	idic_destroy(idic);

	return EXIT_SUCCESS;
}

// Main function

static const int ntests = 4;

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
		case 4 : return test4(argc-1,argv+1);
	}
	return(EXIT_FAILURE); // shouldn't get here!
}
