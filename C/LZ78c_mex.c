#include "LZ78c_matlab.h"

// Main function

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	const size_t n = mxGetNumberOfElements(prhs[0]); // string length
	char* const istr = mxArrayToString(prhs[0]);     // create input cstring
	const size_t dlen = 2*n;                         // the dictionary length
	char* const dbuf = mxMalloc(dlen);               // allocate dictionary storage buffer
	const size_t c = LZ78c(istr,dbuf,dlen);          // LZ78 algorithm: build the dictionary
	mxFree(istr);                                    // deallocate cstring
	plhs[0] = mxCreateDoubleScalar((double)c);       // output LZ complexity (size of dictionary)
	if (nlhs > 1) plhs[1] = dict_to_cvec(dbuf,c);    // optionally output dictionary as cell vector of strings
	mxFree(dbuf);                                    // deallocate dictionary
}
