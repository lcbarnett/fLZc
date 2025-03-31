#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <assert.h>

#include "mt64.h"

#define DSEPCHAR ('.')
#define DLEN(n)  (2*(n)+2)

// "Diagnostic printf" macro: printf to stderr if NDEBUG not defined

#ifdef NDEBUG
#define dprintf(...)
#else
#define dprintf(...) fprintf(stderr,__VA_ARGS__)
#endif

#define UNUSED __attribute__ ((unused))

void tfmt(char* const tstr, const size_t tsmaxlen, const double t /* secs */);

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

#endif // UTILS_H
