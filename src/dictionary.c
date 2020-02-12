#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "dictionary.h"

struct dictionary *dictionary_load(const char *file_name, size_t size)
{
	FILE *file = fopen(file_name, "r");
	struct stat file_stats;
	struct dictionary *dictionary = malloc(sizeof(*dictionary));
	int i = 0, c = 0, ch;
	
	if (file == NULL)
		goto ERROR; 

	if (fstat(fileno(file), &file_stats) == -1)
		goto ERROR;

	dictionary->data = malloc(size * sizeof(*(dictionary->data)));
	memset(dictionary->data, '\0', size);

	*(dictionary->data) = malloc(file_stats.st_size);
	memset(*(dictionary->data), '\0', file_stats.st_size);
	dictionary->size = size;

	while ((ch = fgetc(file)) != EOF) {
		if (ch == '\n') {
			dictionary->data[i][c] = '\0';

			if (i + 1 > dictionary->size) {
				dictionary->data = realloc(
						dictionary->data,
						dictionary->size * 1.5 * sizeof(*(dictionary->data))
				);
				memset(dictionary->data + size, '\0', (size * 1.5) - size);
				dictionary->size *= 1.5;
			}

			dictionary->data[i + 1] = &dictionary->data[i][c + 1];
			c = 0;
			i++;
		} else {
			dictionary->data[i][c++] = ch;
		}
	}

	fclose(file);
	return dictionary;

ERROR:
	perror("Error loading dictionary");
	fclose(file);
	free(dictionary);
	return NULL;
}

void dictionary_close(struct dictionary *dictionary)
{
	free(*(dictionary->data));
	free(dictionary->data);
	free(dictionary);
}
