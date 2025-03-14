#include "LZ78c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
	char* const str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const char sepchar = '|';


	const size_t dlen = 2*n;
	char* const dbuf = malloc(dlen);
	size_t* const cc = malloc(n*sizeof(size_t));
	LZ78c_x(str,dbuf,dlen,cc);
	const size_t c = cc[n-1];
	printf("*** complexity = %lu\n\n",c);
	dict_print(dbuf,c,sepchar);
	printf("\n\n");
	free(dbuf);

	for (size_t i=0; i<n; ++i)printf("%8zu\n",cc[i]);
	printf("\n");

	free(cc);
	free(str);

	return EXIT_SUCCESS;
}
