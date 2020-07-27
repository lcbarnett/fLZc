#ifndef LZC_H
#define LZC_H

/*
	TODO
*/

#define UNUSED __attribute__ ((unused))

#define DTRMCHAR  CHAR_MAX
#define DSEPCHAR '|'

#ifdef GLIBVER

#include <gmodule.h>

#ifdef DEBUG
static const char idstr[] = "GLIB (DEBUG)";
#else
static const char idstr[] = "GLIB";
#endif

#else // KHASH

#ifdef DEBUG
static const char idstr[] = "KHASH (DEBUG)";
#else
static const char idstr[] = "KHASH";
#endif

#include "khash.h"

KHASH_SET_INIT_STR(str)

#endif // GLIBVER/KHASH

size_t maxwordlen(const size_t n);

size_t sdlen(const size_t n, const size_t d);

size_t cmax(const size_t n,const size_t d);

void cmax_x(size_t* const c, const size_t nmax, const size_t d);

void dict_to_str(char* const dict);

void dict_print(const char* const dict);

size_t LZc(const char* const istr, char* const dict);

void LZc_x(const char* const istr, char* const dict, size_t* const c);

// size_t LZ76_KANDS(const char* const istr, const size_t n);
size_t LZ76_ref(const char* const istr, const size_t n);
size_t LZ76(const char* const istr, const size_t n);
void LZ76_x(const char* const istr, const size_t n, size_t* const c);

#endif // LZC_H
