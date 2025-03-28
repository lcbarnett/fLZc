#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "LZ78c.h"

// Static dictionary

size_t LZ78c_sd(const char* const str, char* const sdic, const size_t sdlen)
{
	// LZ78c - static dictionary
	//
	// str MUST be NUL-terminated.
	//
	// sdic MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.

	strset_t* const dict = strset_init();    // create and initialise dictionary
	memset(sdic,0,sdlen);                    // NUL-fill the dictionary storage buffer
	char* word = sdic;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 = '\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
	}
	const size_t c = kh_size(dict);          // LZ78c = size of dictionary
	strset_destroy(dict);                    // destroy dictionary
	return c;
}

void LZ78c_sd_x(const char* const str, char* const sdic, const size_t sdlen, size_t* const c)
{
	// LZ78c - static dictionary
	//
	// str MUST be NUL-terminated.
	//
	// sdic MUST be large enough - this code does not check! Safe but
	// pessimistic case is twice the string length.
	//
	// c MUST be same size as the input string.

	strset_t* const dict = strset_init();    // create and initialise dictionary
	memset(sdic,0,sdlen);                    // NUL-fill the dictionary storage buffer
	char* word = sdic;                       // initialise pointer to beginning of current word
	char* w = word;                          // pointer to end of current word (null terminator)
	int added;                               // flag for strset_put
	for (const char* p = str; *p; ++p) {     // traverse input string (terminating condition equiv to *p == 0 ='\0' !!!)
		*w++ = *p;                           // append next input character to word
		strset_put(dict,word,&added);        // add word to dictionary if not already there
		if (added) word = ++w;               // skip past NUL and reinitialise to empty word
		c[p-str] = kh_size(dict);            // set current complexity to dictionary size
	}
	strset_destroy(dict);                    // destroy dictionary
}

// Dynamic dictionary - hash set (cannot sort)

size_t LZ78c_ds(char* const str, strset_t* const ddic)
{
	// LZ78c - dynamic dictionary (hash set - cannot sort)
	//
	// str MUST be NUL-terminated.
	//
	// hash set ddic must be initialised (allocated) with dd_destroy(), and must be deallocated after use!

	ds_clear(ddic);                                // clear dynamic dictionary
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strset_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash set
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			++w;                                   // extend word to next char
		}
		if (*w == 0) break;                        // finished
	}
	return kh_size(ddic);                          // return LZ78c = size of dictionary

	// remember to call ds_destroy(ddic) !!!
}

void LZ78c_ds_x(char* const str, strset_t* const ddic, size_t* const c)
{
	// LZ78c - dynamic dictionary (hash set - cannot sort)
	//
	// str MUST be NUL-terminated.
	//
	// hash set ddic must be initialised (allocated) with dd_destroy(), and must be deallocated after use!
	//
	// c MUST be same size as the input string.

	ds_clear(ddic);                                // clear dynamic dictionary
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strset_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash set
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				c[w-str-1] = kh_size(ddic);        // set current complexity to dictionary size
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			c[w-str-1] = kh_size(ddic);            // set current complexity to dictionary size
			++w;                                   // extend word to next char
		}
		if (*w == 0) {
			c[w-str-1] = kh_size(ddic);            // set current complexity to dictionary size
			break;                                 // finished
		}
	}

	// remember to call ds_destroy(ddic) !!!
}

// Dynamic dictionary - hash map (can sort)

size_t LZ78c_dm(char* const str, strmap_t* const ddic)
{
	// LZ78c - dynamic dictionary (hash map - can sort)
	//
	// str MUST be NUL-terminated.
	//
	// hash map ddic must be initialised (allocated) with dd_destroy(), and must be deallocated after use!

	dm_clear(ddic);                                // clear dynamic dictionary
	size_t c = 0;                                  // complexity
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strmap_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash map
				kh_val(ddic,k) = c++;              // store complexity in dictionary, and increment
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			++w;                                   // extend word to next char
		}
		if (*w == 0) break;                        // finished
	}
	assert(c == kh_size(ddic));
	return c;                                      // return LZ78c = size of dictionary

	// remember to call dm_destroy(ddic) !!!
}

void LZ78c_dm_x(char* const str, strmap_t* const ddic, size_t* const c)
{
	// LZ78c - dynamic dictionary (hash map - can sort)
	//
	// str MUST be NUL-terminated.
	//
	// hash map ddic must be initialised (allocated) with dd_destroy(), and must be deallocated after use!
	//
	// c MUST be same size as the input string.

	dm_clear(ddic);                                // clear dynamic dictionary
	size_t cc = 0;                                 // complexity
	int added;                                     // flag for strset_put
	char wchar;                                    // temp storage for current word one-past-end
	char* p = str;                                 // current word start
	while (1) {                                    // loop through words
		char* w = p+1;                             // current word one-past-end
		while (1) {                                // loop through characters in word
			wchar = *w;                            // save current word one-past-end
			*w = 0;                                // NUL-terminate current word
			khint_t k = strmap_put(ddic,p,&added); // add current word to dictionary if not already there
			if (added) {                           // wasn't there
				kh_key(ddic,k) = strdup(p);        // copy current word into hash map
				kh_val(ddic,k) = cc++;             // store complexity in dictionary, and increment
				p = w;                             // set current word to next word
				*w = wchar;                        // restore current word one-past-end
				c[w-str-1] = cc;                   // set current complexity
				break;                             // done - on to next word
			}
			*w = wchar;                            // restore current word one-past-end
			if (*w == 0) break;                    // finished
			c[w-str-1] = cc;                       // set current complexity
			++w;                                   // extend word to next char
		}
		c[w-str-1] = cc;                           // set current complexity
		assert(cc == kh_size(ddic));
		if (*w == 0) break;                        // finished
	}

	// remember to call dm_destroy(ddic) !!!
}
