#include "LZ76c_matlab.h"

// Main function

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
	char* const istr = mxArrayToString(prhs[0]);        // create input cstring
	size_t* const ci = mxMalloc(n*sizeof(size_t));      // allocate complexities (integer)
	LZ76c_x(istr,ci);                                   // LZ76 algorithm (extended)
	mxFree(istr);                                       // deallocate cstring
	double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
	for (size_t i=0; i<n; ++i) cd[i] = (double)(ci[i]); // copy integer complexities to (double) output
	mxFree(ci);                                         // deallocate complexities (integer)
}
