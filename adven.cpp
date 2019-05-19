#include <ncurses.h>
#include <cstdlib>

#include "map_loader.h"

#define FILENAME "testing_map.dat"

int main() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	int y,x;
	getmaxyx(stdscr,y,x);

	//int pad_y = y*1.5, pad_x = x*1.5;

	int ypos = 0, xpos = 0; //top left corner of the visible map
	int c_ypos = 2, c_xpos = 2; //position of the player

	map_loader dan;


	int log_width = 40, hp_height = 3; //size of the hud

	int map_start_y = hp_height, map_start_x = 0, map_size_y = y-hp_height, map_size_x = x-log_width; //start and end points for drawing the map

	WINDOW *hp_win = newwin(hp_height,x-log_width,0,0);
	WINDOW *log_win = newwin(y,log_width,0,x-log_width);

	box(hp_win,0,0);
	box(log_win,0,0);

	//wmove(log_win,y-1,1);
	scrollok(log_win,true);
	/*
	scrolling doesn't play nice with a border it scrolls the border too
	idk what people do about that whether it's making a new window inside the border
	or just like implementing scrolling themselves which seems like too much trouble
	*/

	WINDOW *map = dan.load_map(FILENAME);
	
	mvwaddch(map,c_ypos,c_xpos,'@');

	wprintw(log_win,"Welcome to Jason's roguelike, <adven.cpp>!");

	refresh();
	wrefresh(hp_win);
	wrefresh(log_win);
	prefresh(map,ypos,xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
	
	//input loop
	char in;
	do {
		in = getch();

		switch(in) {
		//pickup
		case 'g':
			if(mvwinch(dan.getgrid(),c_ypos,c_xpos) == '$') {
				//inventory management..?
				mvwaddch(dan.getgrid(),c_ypos,c_xpos,'.');
				wprintw(log_win,"\nYou grab the dollar sign!");
			} else {
				wprintw(log_win,"\nNothing here...");
			}
			break;
		//log testing
		case 'l':
			wprintw(log_win,"\nHello there");
			break;
		case 'k':
			wprintw(log_win,"\nGeneral Kenobi");
			break;
		//char movement down here
		case 'w':
			if(mvwinch(map,c_ypos-1,c_xpos) != '#') {
				if(ypos>0 && c_ypos-ypos+map_start_y == map_size_y/2) ypos --;
				mvwaddch(map,c_ypos,c_xpos, mvwinch(dan.getgrid(),c_ypos,c_xpos) );
				c_ypos --;
				mvwaddch(map,c_ypos,c_xpos,'@');
			}
			break;
		case 's':
			if(mvwinch(map,c_ypos+1,c_xpos) != '#') {
				if(ypos+map_size_y < dan.gety() && c_ypos-ypos+map_start_y == map_size_y/2) ypos ++;
				mvwaddch(map,c_ypos,c_xpos, mvwinch(dan.getgrid(),c_ypos,c_xpos) );
				c_ypos ++;
				mvwaddch(map,c_ypos,c_xpos,'@');
			}
			break;
		case 'a':
			if(mvwinch(map,c_ypos,c_xpos-1) != '#') {
				if(xpos>0 && c_xpos-xpos+map_start_x == map_size_x/2) xpos --;
				mvwaddch(map,c_ypos,c_xpos, mvwinch(dan.getgrid(),c_ypos,c_xpos) );
				c_xpos --;
				mvwaddch(map,c_ypos,c_xpos,'@');
			}
			break;
		case 'd':
			if(mvwinch(map,c_ypos,c_xpos+1) != '#') {
				if(xpos+map_size_x < dan.getx() && c_xpos-xpos+map_start_x == map_size_x/2) xpos ++;
				mvwaddch(map,c_ypos,c_xpos, mvwinch(dan.getgrid(),c_ypos,c_xpos) );
				c_xpos ++;
				mvwaddch(map,c_ypos,c_xpos,'@');
			}
			break;
		}

		if(mvwinch(dan.getgrid(),c_ypos,c_xpos) == '$') {
			wprintw(log_win,"\nYou see a dollar sign on the floor. Press 'g' to pick it up.");
		}

		prefresh(map,ypos,xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
		wrefresh(log_win);
		//mvderwin(view,ypos,xpos);
		//wrefresh(view);
	} while(in != 'q');

	//wgetch(view); //wait
	endwin();
	return 0;
}
