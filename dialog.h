#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <ncurses.h>

/*
this will be a 'class' that handles a dialog system with pop-up windows, choices, idk a bunch of stuff
*/

struct dialog_selector { //circular list
	int line; //what line the menu option is on
	dialog_selector *prev;
	dialog_selector *next;
};

struct dialog_win { //this one's a stack ok
	WINDOW *data;
	dialog_selector *selector;
	dialog_win *prev;
};

class dialog {
	dialog_win *top;

	dialog_selector *add_selector(int choices[], int choices_size);
public:
	dialog(WINDOW *win, int choices[], int choices_size);
	void add_win(WINDOW *win, int choices[], int choices_size);
	void pop_win();
	int select();
	void move_up();
	void move_down();
	void refresh_win();
	WINDOW *get_win();
};

#endif
