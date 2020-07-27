#include "LZc_matlab.h"

// Main function

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = (size_t)mxGetScalar(prhs[0]);
	const size_t d = (size_t)mxGetScalar(prhs[1]);
	plhs[0] = mxCreateDoubleScalar((double)cmax(n,d));
}
