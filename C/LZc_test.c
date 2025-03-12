#include "LZc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
	char* const  str = argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]; // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const size_t cref = LZc_ref(str);
	const size_t c    = LZc(str);
	printf("complexity = %zu    %zu\n\n",cref,c);

	size_t* const cc = malloc(n*sizeof(size_t));
	LZc_x(str,cc);
	printf("complexity =\n");
	for (size_t i=0; i<n; ++i) printf("\t%zu\n",cc[i]);
	putchar('\n');
	free(cc);

	return EXIT_SUCCESS;
}
