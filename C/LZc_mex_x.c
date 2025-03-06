#include "LZc_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
	char* const istr = mxArrayToString(prhs[0]);        // create input cstring
	char* const dict = mxCalloc(2*n,1);                 // allocate (NUL-filled) string dictionary
	size_t* const ci = mxMalloc(n*sizeof(size_t));      // allocate complexities (integer)
	LZc_x(istr,dict,ci);                                // LZ algorithm (extended): build the dictionary
	mxFree(istr);                                       // deallocate cstring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(ci[i]); // copy integer complexities to (double) output
	if (nlhs > 1) plhs[1] = dict_to_cvec(dict,ci[n-1]); // optionally output dictionary as cell vector of strings
	mxFree(ci);                                         // deallocate complexities (integer)
	mxFree(dict);
}
