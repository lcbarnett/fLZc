#include <stdlib.h>
#include <string.h>

#include "LZ76c.h"

// Tests

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
		const size_t c1 = LZ76c_ref(str);
		const size_t c2 = LZ76cd(str,dict);
		printf("%6zu %6zu    %s\n",c1,c2,dict);
		str[i+1] = tmp;
	}
	putchar('\n');

	free(dict);
	free(str);

	return EXIT_SUCCESS;
}

int test2(int argc, char* argv[])
{
	// Test running complexity

	char* const str = strdup(argc < 2 ? "0001101001000101" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string a (%lu): '%s'\n\n",n,str);

	size_t* const c1 = malloc(n*sizeof(size_t));

	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		c1[i] = LZ76c_ref(str);
		str[i+1] = tmp;
	}

	size_t* const c2 = malloc(n*sizeof(size_t));
	LZ76cr(str,c2);
	for (size_t i=0; i<n; ++i) {
		printf("%6zu %6zu\n",c1[i],c2[i]);
	}
	putchar('\n');

	free(c2);
	free(c1);
	free(str);

	return EXIT_SUCCESS;
}

int test3(int argc, char* argv[])
{
	// Test running complexity and dictionary

	char* const str = strdup(argc < 2 ? "0001101001000101" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string a (%lu): '%s'\n\n",n,str);

	size_t* const c1 = malloc(n*sizeof(size_t));
	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		c1[i] = LZ76c(str);
		str[i+1] = tmp;
	}

	size_t* const c2 = malloc(n*sizeof(size_t));
	char* const dict = malloc(DLEN(n));
	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		LZ76crd(str,c2,dict);
		for (size_t j=0; j<i; ++j) {
			printf("%6zu %6zu    %s\n",c1[j],c2[j],dict);
		}
		putchar('\n');
		str[i+1] = tmp;
	}
	putchar('\n');

	free(dict);
	free(c2);
	free(c1);
	free(str);

	return EXIT_SUCCESS;
}

// Main function

static const int ntests = 3;

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
