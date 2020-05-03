#include <ncurses.h>
#include <string>
#include <cstdlib>

int charlie() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	int y,x;
	getmaxyx(stdscr,y,x);

	printw("Charlie is awake.");

	getch();
	endwin();
	return 0;
}

int main() {
	return charlie();
}
