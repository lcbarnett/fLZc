#include "LZ78c.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const istr = mxArrayToString(prhs[0]);           // create input cstring
	const size_t n = mxGetNumberOfElements(prhs[0]);       // string length
	const int static_dict = (int)mxGetScalar(prhs[1]);     // number of oscillators
	size_t* const c = malloc(n*sizeof(size_t));            // allocate complexities (integer)
	if (static_dict) {                                     // static dictionary? (else dynamic)
		const size_t sdlen = 2*n;                          // static dictionary length
		char* const sdic = malloc(sdlen);                  // allocate static dictionary
		LZ78cs_x(istr,sdic,sdlen,c);                       // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = sdic_to_cvec(sdic,c[n-1]); // optionally output dictionary as cell vector of strings
		free(sdic);                                        // deallocate static dictionary
	}
	else {                                                 // dynamic dictionary
		strset_t* ddic = strset_init();                    // allocate and initialise dynamic dictionary (hash set)
		LZ78cd_x(istr,ddic,c);                             // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = ddic_to_cvec(ddic);        // optionally output dictionary as cell vector of strings
		dd_destroy(ddic);                                  // deallocate dynamic dictionary
	}
	mxFree(istr);                                          // deallocate cstring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);     // copy integer complexities to (double) output
	free(c);                                               // deallocate complexities (integer)

}
