#ifndef STRDUPT_H
#define STRDUPT_H

#include <string.h>
#include <assert.h>

// Like strdup(), but adds a "tag" character to the end of the copied string

static inline const char* strdupt(const char* const word, const char tag)
{
	// Duplicate word with tag character
	const size_t n = strlen(word);   // length of word
	char* const tword = malloc(n+2); // allocate extra space for the tag character
	strcpy(tword,word);              // copy last word into tagged word buffer
	tword[n] = tag;                  // insert the tag
	tword[n+1] = 0;                  // NUL-terminate
	return tword;                    // remember to deallocate !!!
}

// Some character which definitely shoudn't be in the alphabet!
#define TAGCHAR ('*')

#endif // STRDUPT_H
