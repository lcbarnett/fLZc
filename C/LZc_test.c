#include "LZc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
	char* const  istr = argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]; // input cstring (also try without last A)
	const size_t n = strlen(istr);
	printf("\ninput string (%lu): '%s'\n\n",n,istr);

	char* const dict = calloc(2*n,1);

	const size_t c = LZc(istr,dict);
	printf("\ncomplexity = %lu\n\n",c);

	dict_to_str(dict,c,'|');

	printf("\n%s\n\n",dict);

	free(dict);

	return EXIT_SUCCESS;
}
