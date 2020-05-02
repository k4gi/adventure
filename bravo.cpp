#include "bravo.h"

int bravo() {
	//code goes here buddy
	
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	srand(0); //seed random generator

	int y,x;
	getmaxyx(stdscr,y,x);

	printw("Hello there!\nHow are you?");
	
	getch();
	endwin();
	return 0;
}

int main() {
	return bravo();
}
