#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

struct dictionary {
	char **data;
	size_t words;
	size_t characters;
};

struct dictionary *dictionary_load(const char *file);
int dictionary_get_info(int file, struct dictionary *dictionary);
void dictionary_close(struct dictionary *dictionary);

#endif
