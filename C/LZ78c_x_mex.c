#include "LZ78c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc_x.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]);            // create input string
	const size_t n = mxGetNumberOfElements(prhs[0]);       // string length
	const bool static_dict = (bool)mxGetScalar(prhs[1]);   // static dictionary? (else dynamic)
	size_t* const c = malloc(n*sizeof(size_t));            // allocate LZc's
	if (static_dict) {                                     // static dictionary? (else dynamic)
		const size_t sdlen = 2*n;                          // static dictionary length
		char* const sdic = malloc(sdlen);                  // allocate static dictionary
		LZ78c_sd_x(str,sdic,sdlen,c);                      // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = sd_to_cvec(sdic,c[n-1]);   // optionally output dictionary as cell vector of strings
		free(sdic);                                        // deallocate static dictionary
	}
	else {                                                 // dynamic dictionary
		strset_t* ddic = strset_init();                    // allocate and initialise dynamic dictionary (hash set)
		LZ78c_ds_x(str,ddic,c);                            // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = ds_to_cvec(ddic);          // optionally output dictionary as cell vector of strings
		ds_destroy(ddic);                                  // deallocate dynamic dictionary
	}
	mxFree(str);                                           // deallocate string
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);     // copy integer LZc's to (double) output
	free(c);                                               // deallocate LZc's
}
