#ifndef LZ78C_MATLAB_H
#define LZ78C_MATLAB_H

#include "mex.h"
#include "LZ78c.h"

/*
	TODO
*/

mxArray* dict_to_cvec(const char* const dict, const size_t c);

#endif // LZ78C_MATLAB_H
