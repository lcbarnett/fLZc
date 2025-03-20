#include "LZ76c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]); // create input cstring
	size_t c;                                   // LZ complexity
	if (nlhs > 1) {
		strset_t* ddic = strset_init();         // allocate and initialise dynamic dictionary (hash set)
		int nonx;                               // non-exhaustive flag
		c = LZ76c_d(str,ddic,&nonx);            // LZ76 algorithm with dctionary
		plhs[1] = ddic_to_cvec(ddic);           // output dictionary as cell vector of strings
		dd_destroy(ddic);                       // deallocate dynamic dictionary
		if (nlhs > 2) plhs[2] = mxCreateDoubleScalar((double)nonx);
	}
	else {
		c = LZ76c(str);                          // LZ76 algorithm
	}
	mxFree(str);                                 // deallocate cstring
	plhs[0] = mxCreateDoubleScalar((double)c);   // output LZ complexity
}
