#include<math.h>

#include "utils.h"

void tfmt(char* const tstr, const size_t tsmaxlen, const double t /* secs */)
{
	const double xsecs  = floor(t);          // total whole seconds
	const double xmins  = floor(xsecs/60.0); // total whole minutes
	const double xhrs   = floor(xmins/60.0); // total whole hours
	const double secs   = t - xmins*60.0;
	const int    mins   = (int)floor(xmins - xhrs*60.0);
	const int    hrs    = (int)floor(xhrs);
	snprintf(tstr,tsmaxlen,"%d:%d:%.4f",hrs,mins,secs); // h:m:s
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
