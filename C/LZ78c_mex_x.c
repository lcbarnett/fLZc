#include "LZ78c_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
	char* const istr = mxArrayToString(prhs[0]);        // create input cstring
	const size_t dlen = 2*n;                            // the dictionary length
	char* const dict = mxMalloc(dlen);                  // allocate (NUL-filled) string dictionary
	size_t* const c = mxMalloc(n*sizeof(size_t));       // allocate complexities (integer)
	LZ78c_x(istr,dict,dlen,c);                          // LZ78 algorithm (extended): build the dictionary
	mxFree(istr);                                       // deallocate cstring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer complexities to (double) output
	if (nlhs > 1) plhs[1] = dict_to_cvec(dict,c[n-1]);  // optionally output dictionary as cell vector of strings
	mxFree(c);                                          // deallocate complexities (integer)
	mxFree(dict);                                       // deallocate dictionary
}
