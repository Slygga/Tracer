#include <stdlib.h>
#include <string.h>

#include "tracer.h"

int main(int argc, char **argv)
{
	WINDOW *window = initwin();
	struct dictionary *dictionary = dictionary_load("easy_dictionary");
	struct race *race;
	char results[500];

	do {
		addstr("Press any key to start racing...");
		getch();
		erase();

		race = race_generate(dictionary, 150);
		race_start(race);

		sprintf(
			results,
			"Seconds: %d\nWords: %d\nWPM: %f\nLength: %d\nCorrect Characters: %d\nIncorrect Characters: %d\nAccuracy: %f%%\n",
			race->end_time - race->start_time,
			race->words,
			(float)race->words / (race->end_time - race->start_time) * 60.0,
			race->length,
			race->correct_characters,
			race->incorrect_characters,
			(float)race->correct_characters / (race->correct_characters + race->incorrect_characters) * 100
		);

		erase();
		addstr(results);

		free(race);
		addstr("\nWant to go again? (y|n)\n\n");
	} while (getch() == 'y');

	dictionary_close(dictionary);
	endwin();
	return 0;
}

WINDOW *initwin()
{
	WINDOW *window = initscr();
	box(window, '|', '-');
	move(1, 1);

	start_color();

	init_color(COLOR_GRAY, 15, 15, 15);

	init_pair(INACTIVE_COLOR, COLOR_GRAY, COLOR_BLACK);
	init_pair(ACTIVE_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(VALID_COLOR, COLOR_GREEN, COLOR_BLACK);
	init_pair(INVALID_COLOR, COLOR_RED, COLOR_BLACK);

	cbreak();
	noecho();

	return window;
}

struct race *race_generate(struct dictionary *dictionary, int length)
{
	int current_x, current_y, max_x, max_y;
	struct race *race = malloc(sizeof(*race));
	race->words = 0;
	race->length = length;
	race->correct_characters = 0;
	race->incorrect_characters = 0;

	srand(time(NULL));

	attron(COLOR_PAIR(INACTIVE_COLOR));

	getmaxyx(stdscr, max_y, max_x);

	for (int length_count = 0, random_position = 0; length_count < length; length_count++) {
		getyx(stdscr, current_y, current_x);

		random_position = rand() % dictionary->words;

		if (strlen(dictionary->data[random_position]) + current_x + 1 > max_x) {
			current_x = 0;
			current_y++;
			move(current_y, current_x);
		}

		if (current_x != 0)
			addch(' ');

		addstr(dictionary->data[random_position]);

		srand(time(NULL) * rand());
	}

	attroff(COLOR_PAIR(INACTIVE_COLOR));

	getyx(stdscr, race->end_y, race->end_x);
	move(0, 0);

	return race;
}

void race_start(struct race *race)
{
	int input, expected, x, y, max_x, max_y;

	getmaxyx(stdscr, max_y, max_x);

	race->start_time = time(NULL);

	while ((input = getch()) != 27) { // Escape key
		getyx(stdscr, y, x);

		expected = inch() & A_CHARTEXT;

		race_input_handle(&y, &x, race, input, expected);
		
		if (x >= race->end_x && y >= race->end_y)
			break;

		if (x > max_x) {
			x = 0;
			y++;
		}

		move(y, x);
	}

	race->end_time = time(NULL);
}

void race_input_handle(int *y, int *x, struct race *race, int input, int expected)
{
	int color = INVALID_COLOR, next_character;

	next_character = mvinch(*y, *x + 1);
	move(*y, *x);

	if (input == ' ') {
		if (input == expected && next_character == ' ' || next_character == ERR) {
			*x = 0;
			(*y)++;

			chgat(1, 0, color, NULL);

			return;
		}

		if (input == expected)
			race->correct_characters++;
		else
			race->incorrect_characters++;

		race->words++;
	} else if (input == 8 || input == 127) { // Backspace or Delete respectively
		(*x)--;

		return;
	} else if (input == expected) {
		color = VALID_COLOR;
		race->correct_characters++;
	} else {
		color = INVALID_COLOR;
		race->incorrect_characters++;
	}

	chgat(1, 0, color, NULL);

	(*x)++;
}
