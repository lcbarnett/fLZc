#include "LZ78c_matlab.h"

#include <string.h>

mxArray* sdic_to_cvec(const char* const sdic, const size_t c)
{
	mxArray* const cvec = mxCreateCellMatrix(c,1);
	const char* word = sdic;
	for (size_t k = 0; k < c; ++k) {
		mxSetCell(cvec,k,mxCreateString(word));
		word += (strlen(word)+1); // :-)
	}
	return cvec;
}
