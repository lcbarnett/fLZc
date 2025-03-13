#include "LZc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
//	char* const str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
//	char* const str = strdup(argc < 2 ? "caacbaccaaccababbaacacc" : argv[1]); // input cstring (also try without last A)
	char* const str = strdup(argc < 2 ? "aabbbabbaaababbbabababbbcc" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const size_t dlen = 2*n;
	char* const dict = malloc(dlen);

	const size_t c = LZc(str,dict,dlen);
	printf("complexity = %lu\n\n",c);

	dict_to_str(dict,c,'|');

	printf("%s\n\n",dict);

	free(dict);

	strmap_t* h = LZc_build_dict(str);
	const size_t cc = kh_size(h);
	LZc_destroy_dict(h);

	printf("complexity = %lu\n\n",cc);

	free(str);

	return EXIT_SUCCESS;
}
