#ifndef TRACER_H
#define TRACER_H

#include <ncurses.h>
#include <time.h>

#include "dictionary.h"

#define INACTIVE_COLOR 0
#define ACTIVE_COLOR 1
#define VALID_COLOR 2
#define INVALID_COLOR 3

#define COLOR_GRAY 23


struct race {
	int length;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int words;
	int correct_characters;
	int incorrect_characters;
	time_t start_time;
	time_t end_time;
};


WINDOW *initwin();
struct race *race_generate(struct dictionary *dictionary, int length);
void race_start(struct race *race);
void race_input_handle(int *y, int *x, struct race *race, int input, int expected);

#endif
