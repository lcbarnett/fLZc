#ifndef LZ78C_H
#define LZ78C_H

#define UNUSED __attribute__ ((unused))

#include "utils.h"

// Static dictionary

size_t LZ78c_sd   (const char* const istr, char* const dbuf, const size_t dlen);
void   LZ78c_sd_x (const char* const istr, char* const dbuf, const size_t dlen, size_t* const c);

// Dynamic dictionary (hash set - cannot sort)

size_t LZ78c_ds   (char* const str, strset_t* const ddic);
void   LZ78c_ds_x (char* const str, strset_t* const ddic, size_t* const c);

// Dynamic dictionary (hash map - can sort)

size_t LZ78c_dm   (char* const str, strmap_t* const ddic);
void   LZ78c_dm_x (char* const str, strmap_t* const ddic, size_t* const c);

// Dynamic dictionary (linked list - can sort)

size_t LZ78c_dl(char* const str, ldic_t* const ldic);

#endif // LZ78C_H
