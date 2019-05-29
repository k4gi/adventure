#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <ncurses.h>

/*
this will be a 'class' that handles a dialog system with pop-up windows, choices, idk a bunch of stuff
*/

struct dialog_win { //this one's a stack ok
	WINDOW *data;
	dialog_selector *selector;
	int selected;
	dialog_win *prev;
};

struct dialog_selector { //circular list
	int line;
	dialog_selector *prev;
	dialog_selector *next;
};

class dialog {
	dialog_win *windows;
public:
	dialog(WINDOW *win, int choices[], int choices_size);
	void add_win(WINDOW *win, int choices[], int choices_size);
	void pop_win();
	int select();
	void move_up();
	void move_down();
	WINDOW *get_win();
};

#endif
