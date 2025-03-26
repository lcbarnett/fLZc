#ifndef SLL_H
#define SLL_H

#include <string.h>

// A very basic single-linked list of character strings
// Note that the strings are duplicated into the list,
// which must be destroyed after use.

typedef struct sllnode {
	struct sllnode* next;
	const char*     word;
} sllist_t;

static inline sllist_t* sll_create()
{
	// creates a 'dummy' node; skip when iterating
	sllist_t* const sll = malloc(sizeof(sllist_t));
	sll->word = NULL; // this is just a dummy node
	sll->next = NULL;
	return sll;
}

static inline sllist_t* sll_push(sllist_t* sll, const char* const word)
{
	assert(sll != NULL);
	sll->next = malloc(sizeof(sllist_t));
	sll = sll->next;
	assert(word != NULL);
	sll->word = strdup(word); // this had better be null-terminated :-O
	sll->next = NULL;
	return sll;
}

static inline void sll_print(const sllist_t* const sllroot)
{
	if (sllroot == NULL) return; // nothing to print
	// Note: first entry is a dummy
	for (sllist_t* sll = sllroot->next; sll!= NULL; sll = sll->next) printf("%s\n",sll->word);
}

static inline void sll_destroy(sllist_t* const sllroot)
{
	if (sllroot == NULL) return; // nothing to do
	// first we delete the dummy root node
	sllist_t* sllnext = sllroot->next;
#ifndef NDEBUG
	fprintf(stderr,"freeing dummy node\n");
#endif
	free(sllroot);
	// then we iterate through the other nodes, deleting as we go
	while (sllnext != NULL) {
		sllist_t* const sll = sllnext;
		sllnext = sllnext->next;
#ifndef NDEBUG
		fprintf(stderr,"freeing word [%s]\n",sll->word);
#endif
		assert(sll->word != NULL);
		free((char*)(sll->word));
		free(sll);
	}
}

#endif // SLL_H
