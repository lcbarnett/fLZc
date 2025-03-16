#include "LZ78c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Main function

int main(int argc, char* argv[])
{
	// Demonstrate static vs dynamic dictionaries (see also LZ78c_benchmark.c)

	char* const stra = strdup(argc < 2 ? "ABBBAABABBBABBABAAAABABABBBAA" : argv[1]); // input cstring
	const size_t na = strlen(stra);
	printf("\ninput string a (%lu): '%s'\n\n",na,stra);

	const char sepchar = '|';

	strset_t* ddic = strset_init();

	const size_t sdlena = 2*na;
	char* const sdica = malloc(sdlena);
	size_t* const cc1a = malloc(na*sizeof(size_t));
	LZ78cs_x(stra,sdica,sdlena,cc1a);
	const size_t c1a = cc1a[na-1];
	sdic_print(sdica,c1a,sepchar);
	printf("\n\n");
	free(sdica);

	size_t* const cc2a = malloc(na*sizeof(size_t));
	LZ78cd_x(stra,ddic,cc2a);
	ddic_print(ddic,sepchar);
	printf("\n\n");

	printf("*** complexity a =\n");
	for (size_t i=0; i<na; ++i) printf("%8zu  %8zu\n",cc1a[i],cc2a[i]);
	printf("\n");
	free(cc2a);
	free(cc1a);
	free(stra);

	char* const strb = strdup(argc < 3 ? "00010001110100" : argv[2]); // input cstring
	const size_t nb = strlen(strb);
	printf("\ninput string b (%lu): '%s'\n\n",nb,strb);

	const size_t sdlenb = 2*nb;
	char* const sdicb = malloc(sdlenb);
	size_t* const cc1b = malloc(nb*sizeof(size_t));
	LZ78cs_x(strb,sdicb,sdlenb,cc1b);
	const size_t c1b = cc1b[nb-1];
	sdic_print(sdicb,c1b,sepchar);
	printf("\n\n");
	free(sdicb);

	size_t* const cc2b = malloc(nb*sizeof(size_t));
	LZ78cd_x(strb,ddic,cc2b);
	ddic_print(ddic,sepchar);
	printf("\n\n");

	printf("*** complexity b =\n");
	for (size_t i=0; i<nb; ++i) printf("%8zu  %8zu\n",cc1b[i],cc2b[i]);
	printf("\n");
	free(cc2b);
	free(cc1b);
	free(strb);

	dd_destroy(ddic);

	return EXIT_SUCCESS;
}
