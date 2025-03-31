#include <mex.h>

#include "LZ76c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const  str  = mxArrayToString(prhs[0]);           // create input string
	const size_t n    = mxGetNumberOfElements(prhs[0]);     // string length
	const int    allc = (int)mxGetScalar(prhs[1]);          // extended (return all LZc's)
	if (allc) {                                             // return vector of running complexities
		size_t* const c = malloc(n*sizeof(size_t));         // allocate LZc's
		if (nlhs > 1) {                                     // return dictionary
			char* const dict = malloc(DLEN(n));             // allocate dictionary
			LZ76crd(str,c,dict);                            // LZ76 algorithm
			plhs[1] = mxCreateString(dict);                 // output dictionary as printable string
			free(dict);                                     // deallocate dictionary
		}
		else {                                              // just return LZc's
			LZ76cr(str,c);                                  // LZ76 algorithm
		}
		double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
		for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer LZc's (double) to output
		free(c);                                            // deallocate LZc's
	}
	else {                                                  // just return scalar complexity
		size_t c;
		if (nlhs > 1) {                                     // return dictionary
			char* const dict = malloc(DLEN(n));             // allocate dictionary
			c = LZ76cd(str,dict);                                 // LZ76 algorithm
			plhs[0] = mxCreateDoubleScalar((double)c);      // output scalar LZc
			plhs[1] = mxCreateString(dict);                 // output dictionary as printable string
			free(dict);                                     // deallocate dictionary
		}
		else {                                              // just retrun LZc
			c = LZ76c(str);                                 // LZ76 algorithm
			plhs[0] = mxCreateDoubleScalar((double)c);      // output scalar LZc
		}
	}
	mxFree(str);                                            // deallocate string
}
