#include<stdio.h>

#include <mat.h>

#include "utils.h"

void LZ76c_maxq(const size_t n, double* const cmax, const int d)
{
	// sequence of lengths 1 .. n (cmax must have size n)
	const double logd = log((double)d);
	for (size_t i=1;i<=n;++i) {
		const double logdn = log((double)i)/logd;
		cmax[i-1] = (double)i/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
	}
}

void LZ76c_maxv(size_t* const n, const size_t len, double* const cmax, const int d)
{
	// vector n of lengths; size of vector is len (cmax must have size len)
	const double logd = log((double)d);
	for (size_t i=0;i<len;++i) {
		const double logdn = log((double)n[i])/logd;
		cmax[i] = (double)n[i]/((1.0-2.0*((1.0+log(1.0+logdn)/logd)/logdn))*logdn);
	}
}

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

void sdic_to_str(char* const sdic, const size_t c, const char sepchar)
{
	// replace separating NULs with seperator char
	// c is the size of the dictionary; i.e., the LZc :-)
	char* word = sdic;
	for (size_t k = 0; k < c; ++k) {
		word += strlen(word); // :-)
		*word++ = sepchar;
	}
	*--word = 0; // NUL-terminate string
}

void sdic_print(const char* const sdic, const size_t c, const char sepchar)
{
	// c is the size of the dictionary; i.e., the LZc :-)
	putchar(sepchar);
	size_t k = 0;
	for (const char* word = sdic; k < c; ++k, word += strlen(word)+1) printf("%s%c",word,sepchar);
}

mxArray* sdic_to_cvec(const char* const sdic, const size_t c)
{
	// c is the size of the dictionary; i.e., the LZc :-)
	mxArray* const cvec = mxCreateCellMatrix(c,1); // should be destroyed (somewhere)
	size_t k = 0;
	for (const char* word = sdic; k < c; ++k, word += strlen(word)+1) mxSetCell(cvec,k,mxCreateString(word));
	return cvec;
}

void dds_print(const strset_t* const ddic, const char sepchar)
{
	putchar(sepchar);
	khint_t k;
	kh_foreach(ddic,k) printf("%s%c",kh_key(ddic,k),sepchar);
}

mxArray* dds_to_cvec(const strset_t* const ddic)
{
	mxArray* const cvec = mxCreateCellMatrix(kh_size(ddic),1); // should be destroyed (somewhere)
	khint_t k; // NOTE: khashl iterators are not generally sequential!!!
	mwIndex i = 0;
	kh_foreach(ddic,k) mxSetCell(cvec,i++,mxCreateString(kh_key(ddic,k)));
	return cvec;
}

void ddm_print(const strmap_t* const ddic, const char sepchar)
{
	const size_t c = kh_size(ddic);
	const char** const darray = malloc(c*sizeof(const char*));
	// build sorted array
	khint_t k;
	kh_foreach(ddic,k) darray[kh_val(ddic,k)] = kh_key(ddic,k);
	// print from sorted array
	putchar(sepchar);
	for (size_t i = 0; i < c; ++i) printf("%s%c",darray[i],sepchar);
	free(darray);
}

mxArray* ddm_to_cvec(const strmap_t* const ddic)
{
	const size_t c = kh_size(ddic);
	const char** const darray = malloc(c*sizeof(const char*));
	// build sorted array
	khint_t k;
	kh_foreach(ddic,k) darray[kh_val(ddic,k)] = kh_key(ddic,k);
	// assign to cell array from sorted array
	mxArray* const cvec = mxCreateCellMatrix(kh_size(ddic),1); // should be destroyed (somewhere)
	for (size_t i = 0; i < c; ++i) mxSetCell(cvec,i,mxCreateString(darray[i]));
	free(darray);
	return cvec;
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
