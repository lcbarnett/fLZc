#include "LZ78c_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
	char* const istr = mxArrayToString(prhs[0]);        // create input cstring
	const size_t sdlen = 2*n;                           // static dictionary  length
	char* const sdic = mxMalloc(sdlen);                 // allocate static dictionary
	size_t* const c = mxMalloc(n*sizeof(size_t));       // allocate complexities (integer)
	LZ78cs_x(istr,sdic,sdlen,c);                        // LZ78 algorithm (extended): build the dictionary
	mxFree(istr);                                       // deallocate cstring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer complexities to (double) output
	if (nlhs > 1) plhs[1] = sdic_to_cvec(sdic,c[n-1]);  // optionally output dictionary as cell vector of strings
	mxFree(c);                                          // deallocate complexities (integer)
	mxFree(sdic);                                       // deallocate static dictionary
}
