#include <ncurses.h>
#include <string>

#include "dialog.h"

int main() {
	initscr();
	noecho();
	curs_set(0);

	int y,x;
	getmaxyx(stdscr,y,x);

	int border = 10;
	int border_two = 5;
	
	int chcs[4] = {2,3,4,6};
	int chcs_size = 4;
	
	WINDOW *hello = newwin(y-border*2,x-border*2,border,border);
	mvwaddstr(hello,0,0,"HELLO THERE I AM COMPUTER");
	mvwaddstr(hello,chcs[0],0,"CHOICE ONE");
	mvwaddstr(hello,chcs[1],0,"CHOICE TWO");
	mvwaddstr(hello,chcs[2],0,"CHOICE THREE");
	mvwaddstr(hello,chcs[3],0,"choice four");

	int chcs2[2] = {1,2};
	int chcs2_size = 2;

	WINDOW *press = newwin(y-(border+border_two)*2,x-(border+border_two)*2,border+border_two,border+border_two);
	mvwaddstr(press,0,0,"choice four selected");
	mvwaddstr(press,chcs2[0],0,"piss about");
	mvwaddstr(press,chcs2[1],0,"go back");

	int level = 0;


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
		case '\n':
			mvwprintw(talk.get_win(), 0, 0, "Arbitrary code %d", talk.select());
			switch(level) {
			case 0:
				switch(talk.select()) {
				case 6:
					talk.add_win(press,chcs2,chcs2_size);
					level = 1;
					break;
				}
				break;
			case 1:
				switch(talk.select()) {
				case 2:
					talk.pop_win();
					level = 0;
					break;
				}
				break;
			}
			break;
		}
	} while(in != 'q');

	endwin();
	return 0;
}
