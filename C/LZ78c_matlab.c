#include <string.h>

#include "LZ78c_matlab.h"

mxArray* sdic_to_cvec(const char* const sdic, const size_t c)
{
	mxArray* const cvec = mxCreateCellMatrix(c,1); // should be destroyed (somewhere)
	const char* word = sdic;
	for (size_t k = 0; k < c; ++k) {
		mxSetCell(cvec,k,mxCreateString(word));
		word += (strlen(word)+1); // :-)
	}
	return cvec;
}

mxArray* ddic_to_cvec(const strset_t* const ddic)
{
	mxArray* const cvec = mxCreateCellMatrix(kh_size(ddic),1); // should be destroyed (somewhere)
	khint_t k;
	kh_foreach(ddic,k) mxSetCell(cvec,k,mxCreateString(kh_key(ddic,k)));
	return cvec;
}
