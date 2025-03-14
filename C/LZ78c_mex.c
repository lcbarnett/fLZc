#include "LZ78c_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]); // string length
	char* const istr = mxArrayToString(prhs[0]);     // create input cstring
	const size_t sdlen = 2*n;                        // static dictionary length
	char* const sdic = mxMalloc(sdlen);              // allocate static dictionary
	const size_t c = LZ78cs(istr,sdic,sdlen);        // LZ78 algorithm: build the dictionary
	mxFree(istr);                                    // deallocate cstring
	plhs[0] = mxCreateDoubleScalar((double)c);       // output LZ complexity (size of dictionary)
	if (nlhs > 1) plhs[1] = sdic_to_cvec(sdic,c);    // optionally output dictionary as cell vector of strings
	mxFree(sdic);                                    // deallocate static dictionary
}
