#ifndef LZC76_H
#define LZC76_H

#define UNUSED __attribute__ ((unused))

#include "utils.h"

size_t LZ76c   (const char* const str);
size_t LZ76cd  (const char* const str, char*   const dict);
void   LZ76cr  (const char* const str, size_t* const c);
void   LZ76crd (const char* const str, size_t* const c, char* const dict);

// for testing

size_t LZ76c_ref  (const char* const str);

#endif // LZC76_H
