#include "LZc_matlab.h"

// Main function

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray UNUSED *prhs[])
{
	plhs[0] = mxCreateString(idstr);
}
