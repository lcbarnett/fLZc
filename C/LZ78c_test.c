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

// Main function

static const int ntests = 1;

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
	}
	return(EXIT_FAILURE); // shouldn't get here!
}
