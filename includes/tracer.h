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
	int length; // amount of words in the whole race
	int start_x; // Reserved | Not used
	int start_y; // Reserved | Not used
	int end_x; // X coordinate of the window where the race is done
	int end_y; // Y coordinate of the window where the race is done
	int words; // amount of words typed by the user
	int correct_characters; // amount of correct chacters input by the user
	int incorrect_characters; // amount of incorrect chacters input by the user
	time_t start_time; // time the race started
	time_t end_time; // time the race ended
};


/*
 * Calls the ncurses' initscr() as well as sets up color and other default
 * values.
 *
 * Returns the window similar to initscr()
 */
WINDOW *initwin();

/*
 * Uses an allocated and populated dictionary to populate a race of length words. Words
 * are chosen at random.
 *
 * Returns an allocated race struct with default values;
 */
struct race *race_generate(struct dictionary *dictionary, int length);

/*
 * Accepts a pre-allocated race struct that is used to keep track of
 * the user as they do the race including words, in/correct_characters, etc.
 */
void race_start(struct race *race);

/*
 * Handles the user input and determines the new x and y coordinates the window
 * should refresh to.
 */
void race_input_handle(int *y, int *x, struct race *race, int input, int expected);

#endif
