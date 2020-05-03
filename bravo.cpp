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

	//printw("Hello there!\nHow are you?");

	//define a grid
	int grid_y = 8;
	int grid_x = 8;

	char pc = '@';
	int pc_y = 4;
	int pc_x = 4;

	char input = ' ';
	while( input != 'q' ) {
		for( int i=0; i < grid_y; i++ ) {
			for( int j=0; j < grid_x; j++ ) {
				//wowow here we are printing floors and players and nothing else
				if( i == pc_y && j == pc_x) {
					mvaddch(j,i,'q');
				} else {
					mvaddch(j,i,'.');
				}
			}
		}

		input = getch();

		switch( input ) {
case 'w':		if( pc_y > 0 ) {
				pc_y -= 1;
			}
			break;

case 's':		if( pc_y < grid_y-1 ) {
				pc_y += 1;
			}
			break;

case 'a':		if( pc_x > 0 ) {
				pc_x -= 1;
			}
			break;

case 'd':		if( pc_x < grid_x-1 ) {
				pc_x += 1;
			}
			break;
		}

	}

	//getch();
	endwin();
	return 0;
}

int main() {
	return bravo();
}
