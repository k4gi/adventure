#include <ncurses.h>
#include <string>

#include "dialog.h"

int main() {
	initscr();

	int y,x;
	getmaxyx(stdscr,y,x);

	int border = 10;
	
	WINDOW *hello = newwin(y-border*2,x-border*2,border,border);
	mvwaddstr(hello,0,0,"HELLO THERE I AM COMPUTER");
	mvwaddstr(hello,10,0,"CHOICE ONE");
	mvwaddstr(hello,11,0,"CHOICE TWO");
	mvwaddstr(hello,12,0,"CHOICE THREE");
	mvwaddstr(hello,15,0,"choice four");

	int chcs[4] = {10,11,12,15};
	int chcs_size = 4;

	dialog talk = dialog(hello, chcs, chcs_size);

	char in = 'q';
	do{
		talk.refresh_win();
		in = wgetch(talk.get_win());
		switch(in) {
		case 'w':
			talk.move_up();
			break;
		case 's':
			talk.move_down();
			break;
		}
	} while(in != 'q');

	endwin();
	return 0;
}
