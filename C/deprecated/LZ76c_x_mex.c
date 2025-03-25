#include "LZ76c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc_x.m

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
	char* const str = mxArrayToString(prhs[0]);         // create input string
	size_t* const c = mxMalloc(n*sizeof(size_t));       // allocate complexities (integer)
	LZ76c_x(str,c);                                     // LZ76 algorithm (extended)
	mxFree(str);                                        // deallocate ctring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer LZc's to (double) output
	mxFree(c);                                          // deallocate LZc's
}
