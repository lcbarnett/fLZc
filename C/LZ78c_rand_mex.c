#include "LZ78c_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t   n    = (size_t)   mxGetScalar(prhs[0]);
	const int      d    = (int)      mxGetScalar(prhs[1]);
	const size_t   N    = (size_t)   mxGetScalar(prhs[2]);
	const mtuint_t seed = (mtuint_t) mxGetScalar(prhs[3]);

	if (nlhs > 1) {
		double csdev;
		const double cmean = LZ78cs_rand(n,d,N,seed,&csdev);
		plhs[0] = mxCreateDoubleScalar(cmean); // allocate output value
		plhs[1] = mxCreateDoubleScalar(csdev); // allocate output value
	}
	else {
		const double cmean = LZ78cs_rand(n,d,N,seed,NULL);
		plhs[0] = mxCreateDoubleScalar(cmean); // allocate output value
	}
}
