#include<stdlib.h>
#include<math.h>

#include "utils.h"

void fprintft(FILE* const fs, const double t) /* secs */
{
	long ms,secs,mins,hrs;
	ms   = lrint(1000.0*t);
	secs = ms/1000; ms    %= 1000;
	mins = secs/60; secs  %= 60;
	hrs  = mins/60; mins  %= 60;
	fprintf(fs,"%ld:%ld:%ld.%ld",hrs,mins,secs,ms); // hrs:mins:secs.ms
}

char* sprintft(const double t) /* secs */
{
	long ms,secs,mins,hrs;
	ms   = lrint(1000.0*t);
	secs = ms/1000; ms    %= 1000;
	mins = secs/60; secs  %= 60;
	hrs  = mins/60; mins  %= 60;
	const size_t tsmaxlen = 16; // 15 is enough for 10 years, 59 minutes, 59 seconds and 999 milliseconds :-)
	char* const tbuf = malloc(tsmaxlen+1);
	snprintf(tbuf,tsmaxlen,"%ld:%ld:%ld.%ld",hrs,mins,secs,ms); // hrs:mins:secs.ms
	return tbuf;
	// WARNING: caller must free returned char pointer!!!
}

size_t dmaxlen(const size_t n, const int asize)
{
	// Return maximum dictionary size for strings of length n and
	// alphabet size asize. If dictionary size is unknown, set asize
	// to zero to use a pessimistic length.

	if (asize == 0) return 2*n+2; // pessimistic length

	size_t a  = (size_t)asize;
	size_t g  = 0; // g_k(a)     = a + 2*a^2 + ... + k*a^k
	size_t f  = 0; // f_{k-1}(a) = a + a^2 + ... + a^{k-1}
	size_t ak = 1; // a^k
	size_t k  = 1;
	size_t n0;
	while (1) {
		ak *= a;
		n0  = g;          // the D = k' = 0 string length
		g  += k*ak;
		if (g > n) break; // overshot (note k has also overshot!)
		f += ak;
		++k;
	}
	const double D = floor((double)(n-n0)/(double)k);
	 // max length = n + f_{k-1}(a) + D + 2
	return n + f + (size_t)D + 2;
}

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng)
{
	// Create a uniform random string on a dictionary of a consecutive characters starting at aoff
	// NOTE: string buffer `str' MUST have length > n (probably n+1)!!!
	const double aa = (double)a;
	const double ao = (double)aoff;
	for (size_t i=0; i<n; ++i) str[i] = (char)(ao+aa*mt_rand(prng));
	str[n] = 0; // NUL-terminate
}
