#ifndef LZC76_H
#define LZC76	_H

#define UNUSED __attribute__ ((unused))

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include "mt64.h"

static inline double LZ76c_max(const size_t n, const int d)
{
	// single length n
	const double logd = log((double)d);
	const double logdn = log((double)n)/logd;
	return (double)n/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
}

static inline void LZ76c_maxq(const size_t n, double* const cmax, const int d)
{
	// sequence of lengths 1 .. n (cmax must have size n)
	const double logd = log((double)d);
	for (size_t i=1;i<=n;++i) {
		const double logdn = log((double)i)/logd;
		cmax[i-1] = (double)i/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
	}
}

static inline void LZ76c_maxv(size_t* const n, const size_t len, double* const cmax, const int d)
{
	// vector n of lengths; size of vector is len (cmax must have size len)
	const double logd = log((double)d);
	for (size_t i=0;i<len;++i) {
		const double logdn = log((double)n[i])/logd;
		cmax[i] = (double)n[i]/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
	}
}

size_t LZ76c_ks(const char* const str);
size_t LZ76c_wp(const char* const str);
size_t LZ76c   (const char* const str);

void LZ76c_x(const char* const str, size_t* const c);

double LZ76c_rand(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const csdev);

void LZ76c_rand_x(const size_t n, const int d, const size_t N, const mtuint_t seed, double* const cmean, double* const csdev);

#endif // LZC76_H
