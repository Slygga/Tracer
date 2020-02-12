#include <stdlib.h>

#include "tracer.h"

int main(int argc, char **argv)
{
	WINDOW *window = initwin();
	struct dictionary *dictionary = dictionary_load("data", 3000);
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
			"Start Time: %d\nEnd Time: %d\nSeconds: %d\nWords: %d\nWPM: %f\nLength: %d\nCorrect Characters: %d\nIncorrect Characters: %d\n",
			race->start_time,
			race->end_time,
			race->end_time - race->start_time,
			race->words,
			(float)race->words / (race->end_time - race->start_time) * 60.0,
			race->length,
			race->correct_characters,
			race->incorrect_characters
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
	struct race *race = malloc(sizeof(*race));
	race->words = 0;
	race->length = length;
	race->correct_characters = 0;
	race->incorrect_characters = 0;

	time_t t;
	srand(time(&t));

	attron(COLOR_PAIR(INACTIVE_COLOR));

	for (int i = 0, p = 0; i < length; i++) {
		p = rand() % dictionary->size;

		addstr(dictionary->data[p]);
		addch(' ');

		srand(time(&t) * rand());
	}

	attroff(COLOR_PAIR(INACTIVE_COLOR));

	getyx(stdscr, race->end_y, race->end_x);
	move(0, 0);

	return race;
}

void race_start(struct race *race)
{
	int input,
		expected,
		current_x,
		current_y,
		max_x,
		max_y,
		color;
	time_t start, end;

	time(&start);

	while ((input = getch()) != 27) { // Escape key
		getmaxyx(stdscr, max_y, max_x);
		getyx(stdscr, current_y, current_x);

		expected = inch() & A_CHARTEXT;

		if (input == ' ') {
			race->words++;
		} else if (input == 8 || input == 127) { // Backspace or delete
			current_x -= 2;
		} else if (input == expected) {
			color = VALID_COLOR;
			race->correct_characters++;
		} else {
			color = INVALID_COLOR;
			race->incorrect_characters++;
		}

		chgat(1, 0, color, NULL);

		if (current_x + 1 >= max_x) {
			current_x = 0;
			current_y++;
		} else {
			current_x++;
		}
		
		if (current_x >= race->end_x && current_y >= race->end_y)
			break;

		move(current_y, current_x);
	}

	time(&end);

	race->start_time = start;
	race->end_time = end;
}
