#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LZ78c.h"

// Main function

int test1(int argc, char* argv[])
{
	// Demonstrate static vs dynamic dictionaries (see also LZ78c_benchmark.c)

	const char sepchar = '|';

	char* const stra = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring
	const size_t na = strlen(stra);
	printf("\ninput string a (%lu): '%s'\n\n",na,stra);

	const size_t sdlena = 2*na;
	char* const sdica = malloc(sdlena);
	size_t* const cc1a = malloc(na*sizeof(size_t));
	LZ78c_sd_x(stra,sdica,sdlena,cc1a);
	const size_t c1a = cc1a[na-1];
	sd_print(sdica,c1a,sepchar);
	printf("\n\n");
	free(sdica);

	strmap_t* ddica = strmap_init();
	size_t* const cc2a = malloc(na*sizeof(size_t));
	LZ78c_dm_x(stra,ddica,cc2a);
	dm_print(ddica,sepchar);
	printf("\n\n");
	dm_destroy(ddica);
	free(stra);

	printf("*** complexity a =\n");
	for (size_t i=0; i<na; ++i) printf("%8zu  %8zu\n",cc1a[i],cc2a[i]);
	printf("\n");
	free(cc2a);
	free(cc1a);

	char* const strb = strdup(argc < 3 ? "00010001110100" : argv[2]); // input cstring
	const size_t nb = strlen(strb);
	printf("\ninput string b (%lu): '%s'\n\n",nb,strb);

	const size_t sdlenb = 2*nb;
	char* const sdicb = malloc(sdlenb);
	size_t* const cc1b = malloc(nb*sizeof(size_t));
	LZ78c_sd_x(strb,sdicb,sdlenb,cc1b);
	const size_t c1b = cc1b[nb-1];
	sd_print(sdicb,c1b,sepchar);
	printf("\n\n");
	free(sdicb);

	strset_t* ddicb = strset_init();
	size_t* const cc2b = malloc(nb*sizeof(size_t));
	LZ78c_ds_x(strb,ddicb,cc2b);
	ds_print(ddicb,sepchar);
	printf("\n\n");
	ds_destroy(ddicb);
	free(strb);

	printf("*** complexity b =\n");
	for (size_t i=0; i<nb; ++i) printf("%8zu  %8zu\n",cc1b[i],cc2b[i]);
	printf("\n");
	free(cc2b);
	free(cc1b);

	return EXIT_SUCCESS;
}

int test2(int argc, char* argv[])
{
	// Demonstrate dynamic (hash set) vs dynamic (hash map) dictionaries

	char* const str = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring
	const size_t n = strlen(str);
	printf("\ninput string (%lu): '%s'\n\n",n,str);

	const char sepchar = '|';

	strset_t* ddica = strset_init();
	const size_t ca = LZ78c_ds(str,ddica);
	printf("*** complexity a = %zu : \n",ca);
	ds_print(ddica,sepchar);
	printf("\n\n");
	ds_destroy(ddica);

	strmap_t* ddicb = strmap_init();
	const size_t cb = LZ78c_dm(str,ddicb);
	printf("*** complexity b = %zu : \n",cb);
	dm_print(ddicb,sepchar);
	printf("\n\n");
	dm_destroy(ddicb);

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
