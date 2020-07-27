#include "LZc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
/*
	if (argc < 3) {
		fprintf(stderr, "Must provide alphabet size and string input\n");
		exit(EXIT_FAILURE);
	}

	printf("\nimplementation = %s\n",idstr);
	const size_t d = (size_t)atoi(argv[1]);         // alphabet size
	printf("\nalphabet size = %lu\n",(unsigned long)d);
	char* const  istr = argv[2];                    // input cstring
	printf("\ninput string: '%s'\n",argv[2]);
	const size_t n = strlen(istr);                  // string length
	const size_t dsize = sdlen(n,d);
	printf("\ndictionary size = %lu\n",(unsigned long)dsize);
	char* const dict = calloc(dsize,1);             // allocate (NUL-filled) string dictionary
	size_t* const c = malloc(n*sizeof(size_t));     // allocate working complexity buffer (integer)
	LZc_x(istr,dict,c);                             // complexity
	printf("\ncomplexity = \n");
	for (size_t i=0; i<n; ++i) printf("%lu\n",(unsigned long)c[i]);
	free(c);                                        // deallocate complexity buffer
	printf("\ndictionary: ");
    dict_print(dict);
	dict_to_str(dict);
	printf("\ndictionary:  %s\n\n",dict);
	free(dict);                                     // deallocate string dictionary
*/
	if (argc < 2) {
		fprintf(stderr, "Must provide string input\n");
		return EXIT_FAILURE;
	}

// Try this string, and also without the last A
//
// ABBBAABABBBABBABAAAABABABBBAA

	printf("\nimplementation = %s\n",idstr);
	char* const  istr = argv[1];                    // input cstring
	const size_t n = strlen(istr);
	printf("\ninput string (%lu): '%s'\n\n",n,istr);

	const size_t cref = LZ76_ref(istr,n);
	printf("\ncomplexity (ref) = %lu\n\n",cref);

	const size_t c = LZ76(istr,n);
	printf("\ncomplexity = %lu\n\n",c);

	size_t* const cx = malloc(n*sizeof(size_t));
	LZ76_x(istr,n,cx);
	const size_t cxx = (n ? cx[n-1] : 0);
	printf("\ncomplexity (x) = %lu\n\n",cxx);
	for (size_t i=0;i<n;++i) printf("%3lu : %3lu\n",i+1,cx[i]);
	free(cx);

	return EXIT_SUCCESS;
}
