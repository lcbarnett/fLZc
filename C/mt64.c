#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef WIN
#include <windows.h>
#endif // WIN

#include "mt64.h"

#define PEEXIT(str) {fprintf(stderr,str); perror(NULL); exit(EXIT_FAILURE);}

#define NN 312
#define MM 156
#define MATRIX_A UINT64_C(0xB5026F5AA96619E9)
#define UM UINT64_C(0xFFFFFFFF80000000) // Most  significant 33 bits
#define LM UINT64_C(0x7FFFFFFF)         // Least significant 31 bits

// initializes state
mtuint_t mt_seed(mt_t* const pstate, mtuint_t seed)
{
//	You MUST seed the rng before it is used!!!

	if (seed == 0) { // initialise from /dev/urandom (Unix) or wall time (Windows) for non-repeatability
#ifdef WIN
		// no /dev/urandom on Windows - use mangled (microsecond) wall time
		FILETIME ft;
		long long* t64 = (long long*)&ft;
		GetSystemTimeAsFileTime(&ft);
		seed = (mtuint_t)((*t64-116444736000000000LL)/10);
		// do some 64-bit mixing (from MurmurHash3, as it happens)
		seed ^= (seed >> 33);
		seed *= 0xff51afd7ed558ccd;
		seed ^= (seed >> 33);
		seed *= 0xc4ceb9fe1a85ec53;
		seed ^= (seed >> 33);
#else
		FILE* fp = fopen("/dev/urandom","r");
		if (fp == NULL)                              PEEXIT("\nERROR in mt64 (mt_seed): failed to open /dev/urandom: ");
		if (fread(&seed,sizeof(mtuint_t),1,fp) != 1) PEEXIT("\nERROR in mt64 (mt_seed): failed to read /dev/urandom: ");
		if (fclose(fp) != 0)                         PEEXIT("\nERROR in mt64 (mt_seed): failed to close /dev/urandom: ");
#endif // WIN
	}

	pstate->mt[0] = seed;
	for (pstate->mti=1; pstate->mti<NN; pstate->mti++)
		pstate->mt[pstate->mti] = (UINT64_C(6364136223846793005) * (pstate->mt[pstate->mti-1] ^ (pstate->mt[pstate->mti-1] >> 62)) + (mtuint_t)pstate->mti);

	pstate->iset = 0;
	pstate->gset = 0.0;

	return seed;

}

// generates a 64-bit unsigned integer pseudo-random variate uniform on [0, 2^64-1]-interval
mtuint_t mt_uint64(mt_t* const pstate)
{
	int i;
	mtuint_t x;
	static const mtuint_t mag01[2]={UINT64_C(0), MATRIX_A};

	if (pstate->mti >= NN) { // generate NN words at one time

		// if init_genrand64() has not been called, a default initial seed is used
		// if (pstate->mti == NN+1) mt64_seed(pstate,UINT64_C(5489));
		// No! We don't want this. The user MUST seed first! (probably segfault if they forget)

		for (i=0;i<NN-MM;i++) {
			x = (pstate->mt[i]&UM)|(pstate->mt[i+1]&LM);
			pstate->mt[i] = pstate->mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];
		}
		for (;i<NN-1;i++) {
			x = (pstate->mt[i]&UM)|(pstate->mt[i+1]&LM);
			pstate->mt[i] = pstate->mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];
		}
		x = (pstate->mt[NN-1]&UM)|(pstate->mt[0]&LM);
		pstate->mt[NN-1] = pstate->mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&UINT64_C(1))];

		pstate->mti = 0;
	}

	x = pstate->mt[pstate->mti++];

	x ^= (x >> 29) & UINT64_C(0x5555555555555555);
	x ^= (x << 17) & UINT64_C(0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C(0xFFF7EEE000000000);
	x ^= (x >> 43);

	return x;
}

#undef NN
#undef MM
#undef MATRIX_A
#undef UM
#undef LM
