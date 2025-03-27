#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LZ78c.h"

// Main function

int test1(int argc, char* argv[])
{
	// Demonstrate dynamic (hash set) vs dynamic (hash map) vs dynamic (linked list) dictionaries

	char* const str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const char sepchar = '|';

	strset_t* const ddica = strset_init();
	const size_t ca = LZ78c_ds(str,ddica);
	printf("*** complexity a = %zu : ",ca);
	ds_print(ddica,sepchar);
	printf("\n\n");
	ds_destroy(ddica);

	strmap_t* const ddicb = strmap_init();
	const size_t cb = LZ78c_dm(str,ddicb);
	printf("*** complexity b = %zu : ",cb);
	dm_print(ddicb,sepchar);
	printf("\n\n");
	dm_destroy(ddicb);


	ldic_t* const ddicc = dl_create();
	const size_t cc = LZ78c_dl(str,ddicc);
	printf("*** complexity b = %zu : ",cc);
	dl_print(ddicc,sepchar);
	printf("\n\n");
	dl_destroy(ddicc);

	return EXIT_SUCCESS;
}

int test2(int argc, char* argv[])
{
	// Demonstrate extended static vs dynamic dictionaries vs dynamic (linked list) (see also LZ78c_benchmark.c)

	const char sepchar = '|';

	char* const str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string a (%lu): '%s'\n\n",n,str);

	const size_t sdlen = 2*n;
	char* const sdic = malloc(sdlen);
	size_t* const cc1 = malloc(n*sizeof(size_t));
	LZ78c_sd_x(str,sdic,sdlen,cc1);
	const size_t c1a = cc1[n-1];
	sd_print(sdic,c1a,sepchar);
	printf("\n\n");
	free(sdic);

	strmap_t* const ddic = strmap_init();
	size_t* const cc2 = malloc(n*sizeof(size_t));
	LZ78c_dm_x(str,ddic,cc2);
	dm_print(ddic,sepchar);
	printf("\n\n");
	dm_destroy(ddic);

	ldic_t* const ldic = dl_create();
	size_t* const cc3 = malloc(n*sizeof(size_t));
	LZ78c_dl_x(str,ldic,cc3);
	dl_print(ldic,sepchar);
	printf("\n\n");
	dl_destroy(ldic);

	printf("*** complexity =\n");
	for (size_t i=0; i<n; ++i) printf("%8zu  %8zu  %8zu\n",cc1[i],cc2[i],cc3[i]);
	printf("\n");
	free(cc2);
	free(cc1);

	free(str);

	return EXIT_SUCCESS;
}

// Main function

static const int ntests = 2;

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
	}
	return(EXIT_FAILURE); // shouldn't get here!
}
