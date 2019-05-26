#include <ncurses.h>
#include <cstdlib>
#include <string>

#include "map_loader.h"
#include "unit.h"
#include "roman.h"

#define FILENAME "testing_map.dat"
#define SPAWN_ATTEMPTS 5

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

	unit_list enemies = unit_list();

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
	
	//input loop
	char in;
	do {
		for(int i=1; i<(x-log_width)-2; i++) {
			mvwaddch(hp_win,1,i,' ');
		}

		mvwprintw(hp_win,1,1,roman(pc.hp).c_str());

		if(mvwinch(dan.getgrid(),pc.ypos,pc.xpos) == '$') {
			wprintw(log_win,"\nYou see a dollar sign on the floor. Press 'g' to pick it up.");
		}
		
		enemies.draw(map);

		//refresh the screen
		prefresh(map,ypos,xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
		wrefresh(log_win);
		wrefresh(hp_win);

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
				switch( attack(log_win, &pc, enemies.find_unit(pc.ypos-1, pc.xpos) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					enemies.delete_unit(pc.ypos-1, pc.xpos);
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
				switch( attack(log_win, &pc, enemies.find_unit(pc.ypos+1, pc.xpos) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					enemies.delete_unit(pc.ypos+1, pc.xpos);
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
				switch( attack(log_win, &pc, enemies.find_unit(pc.ypos, pc.xpos-1) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					enemies.delete_unit(pc.ypos, pc.xpos-1);
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
				switch( attack(log_win, &pc, enemies.find_unit(pc.ypos, pc.xpos+1) ) ) {
				case 0:
					break;
				case 1:
					//game over!
					break;
				case 2:
					enemies.delete_unit(pc.ypos, pc.xpos+1);
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

		//enemy action
		debug(log_win,0,"enemy action");
		unit_node *curr = enemies.gethead();
		if( curr == NULL ) {
			//no enemies, high chance of spawn
			debug(log_win,0,"no enemies");
			if( rand()%4 == 0 ) {
				debug(log_win,0,"spawn first enemy");
				//position is monster room of testing_map.dat
				for(int i=0; i<SPAWN_ATTEMPTS; i++) {
					if( enemies.add_unit(map, 7, 7+rand()%5, 37+rand()%9) == 0) break;
				}
			}	
		} else {
			debug(log_win,0,"some enemies");
			while( curr != NULL ) {
				//move enemy
				debug(log_win,0,"moving an enemy");
				switch(rand()%4) {
				case 0:
					move_enemy(map, dan.getgrid(), &curr->data, curr->data.ypos-1, curr->data.xpos);
					break;
				case 2:
					move_enemy(map, dan.getgrid(), &curr->data, curr->data.ypos+1, curr->data.xpos);
					break;
				case 3:
					move_enemy(map, dan.getgrid(), &curr->data, curr->data.ypos, curr->data.xpos-1);
					break;
				case 1:
					move_enemy(map, dan.getgrid(), &curr->data, curr->data.ypos, curr->data.xpos+1);
					break;
				}
				
				if( curr->next == NULL ) {
					debug(log_win,0,"spawn a new enemy?");
					//chance to spawn new enemy
					if( rand()%20 == 0 ) {
						debug(log_win,0,"spawn another enemy");
						//position is monster room of testing_map.dat
						for(int i=0; i<SPAWN_ATTEMPTS; i++) {
							if( enemies.add_unit(map, 7, 7+rand()%5, 37+rand()%9) == 0) break;
						}
						curr = curr->next;
					}	
				}
				curr = curr->next;
			}
		}
	} while(in != 'q');

	//wgetch(view); //wait
	endwin();
	return 0;
}
