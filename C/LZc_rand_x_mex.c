#include "LZc_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t   n    = (size_t)   mxGetScalar(prhs[0]);
	const int      d    = (int)      mxGetScalar(prhs[1]);
	const size_t   N    = (size_t)   mxGetScalar(prhs[2]);
	const mtuint_t seed = (mtuint_t) mxGetScalar(prhs[3]);

	double* const cmean = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // allocate output array

	double* const csdev = nlhs > 1 ? mxGetPr(plhs[1] = mxCreateDoubleMatrix(n,1,mxREAL)) : NULL; // allocate output array

	LZc_rand_x(n,d,N,seed,cmean,csdev);
}
