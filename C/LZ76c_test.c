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

	strmap_t* ddic = strmap_init();
	const size_t cdic = LZ76c_dm(str,ddic);
	dm_print(ddic,'|');
	dm_destroy(ddic);

	printf("\n\ncomplexity = %zu    %zu    %zu\n\n",cref,c,cdic);
/*
	size_t* const cc = malloc(n*sizeof(size_t));
	LZ76c_x(str,cc);
	printf("complexity =\n");
	for (size_t i=0; i<n; ++i) printf("\t%zu\n",cc[i]);
	putchar('\n');
	free(cc);
*/
	return EXIT_SUCCESS;
}

int test2(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	size_t* const c0 = malloc(n*sizeof(size_t));
	size_t* const c1 = malloc(n*sizeof(size_t));
	size_t* const c2 = malloc(n*sizeof(size_t));

	for (size_t i=0; i<n; ++i) {
		const char tmp = str[i+1];
		str[i+1] = 0; // NUL-terminate at length i
		c0[i] = LZ76c(str);
		str[i+1] = tmp;
	}

	LZ76c_x(str,c1);

	strmap_t* ddic = strmap_init();
	LZ76c_dm_x(str,ddic,c2);
	dm_destroy(ddic);

	for (size_t i=0; i<n; ++i) printf("%4zu %8zu %8zu %8zu\n",i+1,c0[i],c1[i],c2[i]);
	putchar('\n');

	size_t ifail1 = SIZE_MAX;
	for (size_t i=0; i<n; ++i) {
		if (c1[i] != c0[i]) {
			ifail1 = i+1;
			break;
		}
	}
	if (ifail1 < SIZE_MAX) {
		fprintf(stderr,"*** Failure 1 at length %zu\n\n",ifail1);
		return(EXIT_FAILURE);
	}

	size_t ifail2 = SIZE_MAX;
	for (size_t i=0; i<n; ++i) {
		if (c2[i] != c0[i]) {
			ifail2 = i+1;
			break;
		}
	}
	if (ifail2 < SIZE_MAX) {
		fprintf(stderr,"*** Failure 2 at length %zu\n\n",ifail2);
		return(EXIT_FAILURE);
	}


	free(c2);
	free(c1);
	free(c0);
	free(str);

	return EXIT_SUCCESS;
}

int test3(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "00010100001010010010111110001011" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	strmap_t* ddic = strmap_init();

	for (size_t k = 0; k < n; ++k) {
		str[n-k] = 0;
		const size_t c = LZ76c_dm(str,ddic);
		printf("LZ76c = %3zu : ",c);
		dm_print(ddic,'|');
		putchar('\n');
break;
	}
	putchar('\n');

	free(str);
	dm_destroy(ddic);

	return EXIT_SUCCESS;
}

int test4(int argc, char* argv[])
{
	// Try: bin/LZ76c_test 4 Hallo world, how are you today?

	ldic_t* const ldic = dl_create();
	ldic_t* ld = ldic;

	for (int k = 1; k < argc; ++k) ld = dl_add(ld,argv[k]);
	ld = dl_push_tag(ld,"That's it, folks!",TAGCHAR);

	printf("\ndictionary = ");
	dl_print(ldic,'|');
	printf("\n\n");

	dl_destroy(ldic);

	return EXIT_SUCCESS;
}

int test5(int argc, char* argv[])
{
	char* const  str = strdup(argc < 2 ? "0001101001000101" : argv[1]); // input cstring (also try without last A)
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const size_t cref = LZ76c_ref(str);
	const size_t ctry = LZ76c_rpt(str);

	printf("\n\nZ76c = %2zu %2zu\n\n",cref,ctry);

	return EXIT_SUCCESS;
}

// Main function

static const int ntests = 5;

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
		case 5 : return test5(argc-1,argv+1);
	}

	return(EXIT_FAILURE); // shouldn't get here!
}
