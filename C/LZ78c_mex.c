#include "LZ78c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]);          // create input string
	const bool static_dict = (bool)mxGetScalar(prhs[1]); // static dictionary? (else dynamic)
	size_t c;                                            // LZ78c
	if (static_dict) {                                   // static dictionary
		const size_t n = mxGetNumberOfElements(prhs[0]); // string length
		const size_t sdlen = 2*n;                        // static dictionary length
		char* const sdic = malloc(sdlen);                // allocate static dictionary
		c = LZ78cs(str,sdic,sdlen);                      // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = sdic_to_cvec(sdic,c);    // optionally output dictionary as cell vector of strings
		free(sdic);                                      // deallocate static dictionary
	}
	else {                                               // dynamic dictionary
		strset_t* ddic = strset_init();                  // allocate and initialise dynamic dictionary (hash set)
		c = LZ78cd(str,ddic);                            // LZ78 algorithm: build the dictionary
		if (nlhs > 1) plhs[1] = ddic_to_cvec(ddic);      // optionally output dictionary as cell vector of strings
		dd_destroy(ddic);                                // deallocate dynamic dictionary
	}
	mxFree(str);                                         // deallocate string
	plhs[0] = mxCreateDoubleScalar((double)c);           // output LZc
}
