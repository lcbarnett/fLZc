#ifndef LZC_MATLAB_H
#define LZC_MATLAB_H

#include "mex.h"
#include "LZc.h"

/*
	TODO
*/

size_t maxn(const double* const p, const size_t n);

mxArray* dict_to_cvec(const char* const dict, const size_t c);

#endif // LZC_MATLAB_H
