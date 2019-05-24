#include <ncurses.h>
#include <cstdlib>
#include <string>

#include "map_loader.h"
#include "unit.h"

#define FILENAME "testing_map.dat"
#define DEBUGGING true
#define DEBUGGING_LEVEL 1

void debug(WINDOW *win,int level,std::string input) {
	if(DEBUGGING && level >= DEBUGGING_LEVEL) {
		wprintw(win,"\ndebug:%s",input.c_str());
	}
}

int main() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	srand(0); //seed random generator
	
	int y,x;
	getmaxyx(stdscr,y,x);


	int ypos = 0, xpos = 0; //top left corner of the visible map

	unit pc;
	pc.name = "player";
	pc.sym = '@';
	pc.ypos = 2;
	pc.xpos = 2;
	pc.hp = 99;
	pc.dmg = 1;

	unit_node *enemies = NULL;

	map_loader dan;

	int log_width = 40, hp_height = 3; //size of the hud

	int map_start_y = hp_height, map_start_x = 0, map_size_y = y-hp_height, map_size_x = x-log_width; //start and end points for drawing the map

	WINDOW *hp_win = newwin(hp_height,x-log_width,0,0);
	WINDOW *log_win = newwin(y,log_width,0,x-log_width);

	box(hp_win,0,0);
	box(log_win,0,0);

	scrollok(log_win,true);
	/*
	scrolling doesn't play nice with a border it scrolls the border too
	idk what people do about that whether it's making a new window inside the border
	or just like implementing scrolling themselves which seems like too much trouble
	*/

	WINDOW *map = dan.load_map(FILENAME);
	
	mvwaddch(map,pc.ypos,pc.xpos,pc.sym);

	wprintw(log_win,"Welcome to Jason's roguelike, <adven.cpp>!");

	refresh();
	wrefresh(hp_win);
	wrefresh(log_win);
	prefresh(map,ypos,xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
	
	//input loop
	char in;
	do {
		//player input & handling
		in = getch();
		switch(in) {
		//pickup
		case 'g':
			if(mvwinch(dan.getgrid(),pc.ypos,pc.xpos) == '$') {
				//inventory management..?
				mvwaddch(dan.getgrid(),pc.ypos,pc.xpos,'.');
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
			switch( move_player(map, dan.getgrid(), &pc, pc.ypos-1, pc.xpos) ) {
			case 0:
				if(ypos>0 && pc.ypos+1 -ypos +map_start_y == map_size_y/2) ypos --;
				break;
			case 1:
				wprintw(log_win,"\nYou bonk into the wall!");
				break;
			case 2:
				switch( attack(log_win, &pc, find_unit(enemies, pc.ypos-1, pc.xpos) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					if( delete_unit(enemies, pc.ypos-1, pc.xpos) == 1) enemies = NULL;
					mvwaddch(map, pc.ypos-1, pc.xpos, mvwinch(dan.getgrid(), pc.ypos-1, pc.xpos) );
					break;
				default:
					debug(log_win,99,"Something broke!");
				}
				break;
			default:
				debug(log_win,99,"Something broke...");
			}
			break;
		case 's':
			switch( move_player(map, dan.getgrid(), &pc, pc.ypos+1, pc.xpos) ) {
			case 0:
				if(ypos+map_size_y < dan.gety() && pc.ypos-1 -ypos +map_start_y == map_size_y/2) ypos ++;
				break;
			case 1:
				wprintw(log_win,"\nYou bonk into the wall!");
				break;
			case 2:
				switch( attack(log_win, &pc, find_unit(enemies, pc.ypos+1, pc.xpos) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					delete_unit(enemies, pc.ypos+1, pc.xpos);
					mvwaddch(map, pc.ypos+1, pc.xpos, mvwinch(dan.getgrid(), pc.ypos+1, pc.xpos) );
					break;
				default:
					debug(log_win,99,"Something broke!");
				}
				break;
			default:
				debug(log_win,99,"Something broke...");
			}
			break;
		case 'a':
			switch( move_player(map, dan.getgrid(), &pc, pc.ypos, pc.xpos-1) ) {
			case 0:
				if(xpos>0 && pc.xpos+1 -xpos +map_start_x == map_size_x/2) xpos --;
				break;
			case 1:
				wprintw(log_win,"\nYou bonk into the wall!");
				break;
			case 2:
				switch( attack(log_win, &pc, find_unit(enemies, pc.ypos, pc.xpos-1) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					delete_unit(enemies, pc.ypos, pc.xpos-1);
					mvwaddch(map, pc.ypos, pc.xpos-1, mvwinch(dan.getgrid(), pc.ypos, pc.xpos-1) );
					break;
				default:
					debug(log_win,99,"Something broke!");
				}
				break;
			default:
				debug(log_win,99,"Something broke...");
			}
			break;
		case 'd':
			switch( move_player(map, dan.getgrid(), &pc, pc.ypos, pc.xpos+1) ) {
			case 0:
				if(xpos+map_size_x < dan.getx() && pc.xpos-1 -xpos +map_start_x == map_size_x/2) xpos ++;
				break;
			case 1:
				wprintw(log_win,"\nYou bonk into the wall!");
				break;
			case 2:
				switch( attack(log_win, &pc, find_unit(enemies, pc.ypos, pc.xpos+1) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					delete_unit(enemies, pc.ypos, pc.xpos+1);
					mvwaddch(map, pc.ypos, pc.xpos+1, mvwinch(dan.getgrid(), pc.ypos, pc.xpos+1) );
					break;
				default:
					debug(log_win,99,"Something broke!");
				}
				break;
			default:
				debug(log_win,99,"Something broke...");
			}
			break;
		}

		if(mvwinch(dan.getgrid(),pc.ypos,pc.xpos) == '$') {
			wprintw(log_win,"\nYou see a dollar sign on the floor. Press 'g' to pick it up.");
		}

		//enemy action
		debug(log_win,0,"enemy action");
		unit_node *curr = enemies;
		if( curr == NULL ) {
			//no enemies, high chance of spawn
			debug(log_win,0,"no enemies");
			if( rand()%4 == 0 ) {
				debug(log_win,0,"spawn first enemy");
				unit_node *new_enemy = new unit_node;
				new_enemy->data.name = "seven";
				new_enemy->data.sym = '7';
				//position is monster room of testing_map.dat
				new_enemy->data.ypos = 7 + rand()%5;
				new_enemy->data.xpos = 37 + rand()%9;

				new_enemy->data.hp = 2;
				new_enemy->data.dmg = 1;

				new_enemy->next = NULL;

				enemies = new_enemy;

				mvwaddch(map,enemies->data.ypos,enemies->data.xpos,enemies->data.sym);
			}	
		} else {
			debug(log_win,0,"some enemies");
			while( curr != NULL ) {
				//move enemy
				debug(log_win,0,"moving an enemy");
				switch(rand()%4) {
				case 0:
					if(mvwinch(map,curr->data.ypos-1,curr->data.xpos) == '.') {
						mvwaddch(map,curr->data.ypos,curr->data.xpos, mvwinch(dan.getgrid(),curr->data.ypos,curr->data.xpos) );
						curr->data.ypos --;
						mvwaddch(map,curr->data.ypos,curr->data.xpos,curr->data.sym);
					}
					break;
				case 2:
					if(mvwinch(map,curr->data.ypos+1,curr->data.xpos) == '.') {
						mvwaddch(map,curr->data.ypos,curr->data.xpos, mvwinch(dan.getgrid(),curr->data.ypos,curr->data.xpos) );
						curr->data.ypos ++;
						mvwaddch(map,curr->data.ypos,curr->data.xpos,curr->data.sym);
					}
					break;
				case 3:
					if(mvwinch(map,curr->data.ypos,curr->data.xpos-1) == '.') {
						mvwaddch(map,curr->data.ypos,curr->data.xpos, mvwinch(dan.getgrid(),curr->data.ypos,curr->data.xpos) );
						curr->data.xpos --;
						mvwaddch(map,curr->data.ypos,curr->data.xpos,curr->data.sym);
					}
					break;
				case 1:
					if(mvwinch(map,curr->data.ypos,curr->data.xpos+1) == '.') {
						mvwaddch(map,curr->data.ypos,curr->data.xpos, mvwinch(dan.getgrid(),curr->data.ypos,curr->data.xpos) );
						curr->data.xpos ++;
						mvwaddch(map,curr->data.ypos,curr->data.xpos,curr->data.sym);
					}
					break;
				}
				
				if( curr->next == NULL ) {
					debug(log_win,0,"spawn a new enemy?");
					//chance to spawn new enemy
					if( rand()%20 == 0 ) {
						debug(log_win,0,"spawn another enemy");

						unit_node *new_enemy = new unit_node;
						new_enemy->data.name = "seven";
						new_enemy->data.sym = '7';
						//position is monster room of testing_map.dat
						new_enemy->data.ypos = 7 + rand()%5;
						new_enemy->data.xpos = 37 + rand()%9;

						new_enemy->data.hp = 2;
						new_enemy->data.dmg = 1;

						new_enemy->next = NULL;

						curr->next = new_enemy;

						curr = curr->next;

						mvwaddch(map,curr->data.ypos,curr->data.xpos,curr->data.sym);
					}	
				}
				curr = curr->next;
			}
		}

		//refresh the screen
		prefresh(map,ypos,xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
		wrefresh(log_win);
		wrefresh(hp_win);
	} while(in != 'q');

	//wgetch(view); //wait
	endwin();
	return 0;
}
