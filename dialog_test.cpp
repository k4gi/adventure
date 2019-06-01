#include <ncurses.h>
#include <string>

#include "dialog.h"

int main() {
	initscr();

	int y,x;
	getmaxyx(stdscr,y,x);

	int border = 10;
	
	int chcs[4] = {2,3,4,6};
	int chcs_size = 4;
	
	WINDOW *hello = newwin(y-border*2,x-border*2,border,border);
	mvwaddstr(hello,0,0,"HELLO THERE I AM COMPUTER");
	mvwaddstr(hello,chcs[0],0,"CHOICE ONE");
	mvwaddstr(hello,chcs[1],0,"CHOICE TWO");
	mvwaddstr(hello,chcs[2],0,"CHOICE THREE");
	mvwaddstr(hello,chcs[3],0,"choice four");


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
		mvwprintw(talk.get_win(),1,0,"select:%d",talk.select());
	} while(in != 'q');

	endwin();
	return 0;
}
