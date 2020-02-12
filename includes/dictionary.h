#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

struct dictionary {
	char **data;
	size_t size;
};

struct dictionary *dictionary_load(const char *file, size_t size);
void dictionary_close(struct dictionary *dictionary);

#endif
