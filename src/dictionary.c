#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dictionary.h"

struct dictionary *dictionary_load(const char *file_name)
{
	struct dictionary *dictionary = malloc(sizeof(*dictionary));
	int file = open(file_name, O_RDONLY);
	int word_index = 0, character_index = 0;
	char current_character;

	if (file == -1) {
		perror("Error opening file in dictionary_load");
		free(dictionary);
		return NULL;
	}

	dictionary_get_info(file, dictionary);
	
	dictionary->data = malloc(dictionary->words * sizeof(*(dictionary->data)));
	memset(dictionary->data, '\0', dictionary->words);

	*(dictionary->data) = malloc(dictionary->characters);
	memset(*(dictionary->data), '\0', dictionary->characters);

	while (read(file, &current_character, 1)) {
		if (current_character == '\n') {
			dictionary->data[word_index][character_index] = '\0';

			dictionary->data[word_index + 1] = &dictionary->data[word_index][character_index + 1];
			character_index = 0;
			word_index++;
		} else {
			dictionary->data[word_index][character_index++] = current_character;
		}
	}

	close(file);
	return dictionary;
}

int dictionary_get_info(int file, struct dictionary *dictionary)
{
	char current_character;

	while (read(file, &current_character, 1)) {
		if (current_character == ' ' || current_character == '\n')
			dictionary->words++;

		dictionary->characters++;
	}

	lseek(file, 0, SEEK_SET);
	return 1;
}

void dictionary_close(struct dictionary *dictionary)
{
	free(*(dictionary->data));
	free(dictionary->data);
	free(dictionary);
}
