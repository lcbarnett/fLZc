#include "LZ76c.h"

// Main function

void mexFunction(int UNUSED nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]);      // create input cstring
	const size_t c = LZ76c(str);                     // LZ76 algorithm
	mxFree(str);                                     // deallocate cstring
	plhs[0] = mxCreateDoubleScalar((double)c);       // output LZ complexity
}
