#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <ncurses.h>

#define HIGHLIGHTING A_REVERSE

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
	dialog();
	dialog(WINDOW *win, int choices[], int choices_size);
	dialog(int height, int width, std::string lines[], int lines_size, int choices[], int choices_size);
	void add_win(WINDOW *win, int choices[], int choices_size);
	void add_win(int height, int width, std::string lines[], int lines_size, int choices[], int choices_size);
	void pop_win();
	int select();
	void move_up();
	void move_down();
	void refresh_win();
	WINDOW *get_win();
	void decorate(char l, char r, char t, char b, char tl, char tr, char bl, char br);
};

#endif
