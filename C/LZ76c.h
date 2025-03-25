#ifndef LZC76_H
#define LZC76	_H

#define UNUSED __attribute__ ((unused))

#include "utils.h"

size_t LZ76c_ref (const char* const str);
size_t LZ76c     (const char* const str);
void   LZ76c_x   (const char* const str, size_t* const c);
size_t LZ76c_d   (      char* const str, strmap_t* const ddic);

#endif // LZC76_H
