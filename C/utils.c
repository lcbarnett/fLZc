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
	const size_t tsmaxlen = 15; // enough for 10 years, 59 minutes, 59 seconds and 999 milliseconds :-)
	char* const tbuf = malloc(tsmaxlen+1);
	snprintf(tbuf,tsmaxlen,"%ld:%ld:%ld.%ld",hrs,mins,secs,ms); // hrs:mins:secs.ms
	return tbuf;
	// WARNING: caller must free returned char pointer!!!
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
