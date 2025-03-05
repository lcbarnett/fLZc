#ifndef MT64_H
#define MT64_H

// 64-bit Mersenne Twister pseudo-random number generation

#include <inttypes.h>

// #include "cudefs.h"

typedef uint64_t mtuint_t;

#define NN 312
typedef struct {
	mtuint_t mt[NN];
	int      mti;
	int      iset;
	double   gset;
} mt_t;
#undef NN

// initializes state
mtuint_t mt_seed(mt_t* const pstate, mtuint_t seed);

// generates a 64-bit unsigned integer pseudo-random variate uniform on [0, 2^64-1]-interval
mtuint_t mt_uint64(mt_t* const pstate);

// generates a double-precision pseudo-random variate uniform on [0,1)-real-interval
inline double mt_rand(mt_t* const pstate)
{
	return (double)(mt_uint64(pstate) >> 11) * (1.0/9007199254740992.0);
}

// generates a double-precision pseudo-random normal variate ~ N(0,1)
double mt_randn(mt_t* const pstate);

#endif // MT64_H
