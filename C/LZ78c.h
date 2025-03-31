#ifndef LZ78C_H
#define LZ78C_H

#include "utils.h"

// Dictionary terminator - should be non-printable

#define DTRMCHAR (1)

static inline void make_printable(char* const dict)
{
	// Replace NULs with dictionary separator character
	char* w;
	for (w = dict; *w != DTRMCHAR; ++w) if (*w == 0) *w = DSEPCHAR;
	*w = 0; // replace dictionary terminator with NUL
}

size_t LZ78c  (const char* const str, char* const dict);
void   LZ78cr (const char* const str, char* const dict, size_t* const c);

#endif // LZ78C_H
