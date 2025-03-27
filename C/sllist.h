#ifndef SLLIST_H
#define SLLIST_H

#include <string.h>
#include <assert.h>

#include "strdupt.h"

// Linked-list dictionary - a very basic single-linked list of character
// strings. Note that strings are duplicated into the list, which must be
// created with dl_create() and destroyed after use with dl_destroy().

typedef struct ldnode {
	struct ldnode* next;
	const char*    word;
} ldic_t;

static inline ldic_t* dl_create()
{
	// creates a 'dummy' node; skip when iterating
#ifndef NDEBUG
		fprintf(stderr,"adding dummy node\n");
#endif
	ldic_t* const ld = malloc(sizeof(ldic_t));
	ld->word = NULL; // this is just a dummy node
	ld->next = NULL;
	return ld;
}

static inline const char* dl_word(ldic_t* ld)
{
	return ld->word;
}

static inline ldic_t* dl_add(ldic_t* ld, const char* const word)
{
	assert(ld != NULL);
	ld->next = malloc(sizeof(ldic_t));
	ld = ld->next;
	assert(word != NULL);
#ifndef NDEBUG
		fprintf(stderr,"adding word [%s]\n",word);
#endif
	ld->word = strdup(word); // this had better be null-terminated :-O
	ld->next = NULL;
	return ld;
}

static inline ldic_t* dl_push_tag(ldic_t* ld, const char* const word, const char tag)
{
	assert(ld != NULL);
	ld->next = malloc(sizeof(ldic_t));
	ld = ld->next;
	assert(word != NULL);
	ld->word = strdupt(word,tag); // this had better be null-terminated :-O
	ld->next = NULL;
	return ld;
}

static inline void dl_destroy(ldic_t* const ldic)
{
	if (ldic == NULL) return; // nothing to do
	// first we delete the dummy root node
	ldic_t* ldnext = ldic->next;
#ifndef NDEBUG
	fprintf(stderr,"freeing dummy node\n");
#endif
	free(ldic);
	// then we iterate through the other nodes, deleting as we go
	while (ldnext != NULL) {
		ldic_t* const ld = ldnext;
		ldnext = ldnext->next;
		assert(ld->word != NULL);
#ifndef NDEBUG
		fprintf(stderr,"freeing word [%s]\n",ld->word);
#endif
		free((char*)(ld->word));
		free(ld);
	}
}

#endif // SLLIST_H
