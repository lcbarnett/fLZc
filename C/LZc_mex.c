#include "LZc_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]); // string length
	char* const istr = mxArrayToString(prhs[0]);     // create input cstring
	const size_t dlen = 2*n;                         // the dictionary length
	char* const dict = mxMalloc(dlen);               // allocate (NUL-filled) string dictionary
	const size_t c = LZc(istr,dict,dlen);            // LZ algorithm: build the dictionary
	mxFree(istr);                                    // deallocate cstring
	plhs[0] = mxCreateDoubleScalar((double)c);       // output LZ complexity (size of dictionary)
	if (nlhs > 1) plhs[1] = dict_to_cvec(dict,c);    // optionally output dictionary as cell vector of strings
	mxFree(dict);                                    // deallocate dictionary
	mxFree(istr);                                    // deallocate input string
}
