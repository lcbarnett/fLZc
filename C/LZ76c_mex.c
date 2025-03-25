#include "LZ76c.h"

// NOTE: do not call this mex-function directly in your Matlab code; it does no
// input checks whatsoever! Rather, us the Matlab wrapper matlab/LZc.m

void mexFunction(int nlhs, mxArray *plhs[], int UNUSED nrhs, const mxArray *prhs[])
{
	char* const str = mxArrayToString(prhs[0]);             // create input string
	const bool  allc = (bool)mxGetScalar(prhs[1]);          // extended (return all LZc's)
	if (allc) {                                             // return vaector of all complexities
		const size_t n = mxGetNumberOfElements(prhs[0]);    // string length
		size_t* const c = malloc(n*sizeof(size_t));         // allocate LZc's
		if (nlhs > 1) {                                     // return dictionary
			strmap_t* const ddic = strmap_init();           // allocate and initialise dynamic dictionary (hash set)
			LZ76c_dm_x(str,ddic,c);                         // LZ76 algorithm: build the dictionary
			plhs[1] = dm_to_cvec(ddic);                     // output (sorted) dictionary as cell vector of strings
			dm_destroy(ddic);                               // deallocate dynamic dictionary
		}
		else {                                              // don't need to return dictionary
			LZ76c_x(str,c);                                 // LZ76 algorithm: build the dictionary
		}
		double* const cd = mxGetPr(plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL)); // create (double) output vector for complexities
		for (size_t i=0; i<n; ++i) cd[i] = (double)(c[i]);  // copy integer LZc's (double) to output
		free(c);                                            // deallocate LZc's
	}
	else {                                                  // just return scalar complexity
		size_t c;
		if (nlhs > 1) {                                     // return dictionary
			strmap_t* const ddic = strmap_init();           // allocate and initialise dynamic dictionary (hash set)
			c = LZ76c_dm(str,ddic);                         // LZ76 algorithm: build the dictionary
			plhs[1] = dm_to_cvec(ddic);                     // output (sorted) dictionary as cell vector of strings
			dm_destroy(ddic);                               // deallocate dynamic dictionary
		}
		else {                                              // don't need to return dictionary
			c = LZ76c(str);                                 // LZ76 algorithm: build the dictionary
		}
		plhs[0] = mxCreateDoubleScalar((double)c);          // output scalar LZc
	}
	mxFree(str);                                            // deallocate string
}
