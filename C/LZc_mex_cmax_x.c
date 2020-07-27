#include "LZc_matlab.h"

// Main function

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n   = (size_t)mxGetScalar(prhs[0]);
	const size_t d   = (size_t)mxGetScalar(prhs[1]);
	size_t* const ci = mxMalloc(n*sizeof(size_t));
	cmax_x(ci,n,d);
	double* const c  = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL));
	for (size_t i=0; i<n; ++i) c[i] = (double)(ci[i]);
	mxFree(ci);
}
