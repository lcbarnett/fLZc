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

// Print formatted time (hrs:mins:secs.ms)

void fprintft(FILE* const fs, const double t);

static inline void printft(const double t)
{
	fprintft(stdout,t);
}

char* sprintft(const double t); // WARNING: caller must free returned char pointer!!!

void make_random_string(char* const str, const size_t n, const int a, const char aoff, mt_t* const prng);

static inline void progrep(const char* const prefix, const size_t i, const size_t n)
{
	// report progress of iterative sim from 10% - 100%
	if ((i+1)%(n/10)==0) printf("%s%3zu%% complete\n",prefix,10*(i+1)/(n/10));
}

#endif // UTILS_H
