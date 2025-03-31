#include <mex.h>

#include "LZ78c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const  str  = mxArrayToString(prhs[0]);           // create input string
	const size_t n    = mxGetNumberOfElements(prhs[0]);     // string length
	char* const  dict = malloc(DLEN(n));                    // allocate dictionary
	const int    allc = (int)mxGetScalar(prhs[1]);          // extended (return all LZc's)
	if (allc) {                                             // return vector of running complexities
		size_t* const c = malloc(n*sizeof(size_t));         // allocate LZc's
		LZ78cr(str,dict,c);                                 // LZ78 algorithm: running complexities
		double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
		for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer LZc's (double) to output
		free(c);                                            // deallocate LZc's
	}
	else {                                                  // just return scalar complexity
		const size_t c = LZ78c(str,dict);                   // LZ78 algorithm
		plhs[0] = mxCreateDoubleScalar((double)c);          // output scalar LZc
	}
	if (nlhs > 1) {                                         // return dictionary
		make_printable(dict);                               // transform dictionary to printable string
		plhs[1] = mxCreateString(dict);                     // output dictionary as printable string
	}
	free(dict);                                             // deallocate dictionary
	mxFree(str);                                            // deallocate string
}
