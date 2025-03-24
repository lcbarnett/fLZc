#include "LZ76c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]); // create input string
	size_t c;                                   // LZc
	if (nlhs > 1) {
		strmap_t* ddic = strmap_init();         // allocate and initialise dynamic dictionary (hash set)
		c = LZ76c_d(str,ddic);                  // LZ76 algorithm with dictionary
		plhs[1] = ddm_to_cvec(ddic);            // output dictionary as cell vector of strings
		ddm_destroy(ddic);                      // deallocate dynamic dictionary
	}
	else {
		c = LZ76c(str);                         // LZ76 algorithm
	}
	mxFree(str);                                // deallocate string
	plhs[0] = mxCreateDoubleScalar((double)c);  // output LZc
}
