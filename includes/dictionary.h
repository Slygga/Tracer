#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>


struct dictionary {
	char **data; // holds an array of words retrieved from a file
	size_t words; // amount of words in the file (newline or space delimited)
	size_t characters; // amount of characters in the file including newlines and spaces
};

/*
 * Opens a file called file_name that should contain words
 * separated by spaces or newlines to load into dictionary struct.
 *
 * Returns fully populated dictionary struct
 */ 
struct dictionary *dictionary_load(const char *file_name);

/*
 * Accepts a pre-opened file to read and get word and character
 * counts to populate the pre-allocated dictionary.
 *
 * Returns 1 | Reserved for future use
 */
int dictionary_get_info(int file, struct dictionary *dictionary);

/*
 * Free()s all the data in the dictionary struct
 */
void dictionary_close(struct dictionary *dictionary);

#endif
