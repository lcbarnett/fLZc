#ifndef LZ78C_MATLAB_H
#define LZ78C_MATLAB_H

#include <mex.h>

#include "LZ78c.h"

mxArray* sdic_to_cvec(const char* const dict, const size_t c);

mxArray* ddic_to_cvec(const strset_t* const ddic);

#endif // LZ78C_MATLAB_H
