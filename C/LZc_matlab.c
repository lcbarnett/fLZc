#include "LZc_matlab.h"

#include <string.h>

/*
	TODO
*/


size_t maxn(const double* const p, const size_t n)
{
	size_t mxn = 0;
	for (size_t i=0; i<n; ++i) {
		if (p[i] > n) mxn = (size_t)(p[i]);
	}
	return mxn;
}

mxArray* dict_to_cvec(const char* const dict, const size_t c)
{
	mxArray* const cvec = mxCreateCellMatrix(c,1);
	size_t i = 0;
	const char* word = dict;
	while (*word != DTRMCHAR) {
		mxSetCell(cvec,i++,mxCreateString(word));
		word += (strlen(word)+1); // :-)
	}
	return cvec;
}
