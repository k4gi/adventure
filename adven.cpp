#include <ncurses.h>
#include <cstdlib>
#include <string>

#include "map_loader.h"
#include "unit.h"
#include "roman.h"
#include "dialog.h"

#define FILENAME "testing_map.dat"
#define SPAWN_ATTEMPTS 5

#define DEBUGGING true
#define DEBUGGING_LEVEL 2

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

	srand(0); //seed random generator. with zero. every day.
	
	int y,x;
	getmaxyx(stdscr,y,x);


	int ypos = 0, xpos = 0; //top left corner of the visible map

	unit pc;
	pc.name = "player";
	pc.sym = '@';
	pc.ypos = 2;
	pc.xpos = 2;
	pc.hp = 100;
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

	//dialog windows here

	dialog di = dialog();

	int dialog_level = 0; //1 or greater when a dialog is open
	int dialog_height = 6;
	int dialog_width = 40;

	WINDOW *talk_to_rose = newwin(dialog_height, dialog_width, y/2 - dialog_height/2, x/2 - dialog_width/2);
	for(int i=0; i<dialog_width; i++) {
		mvwaddch(talk_to_rose, 0, i, '=');
		mvwaddch(talk_to_rose, 5, i, '=');
	}
	mvwprintw(talk_to_rose, 1, 1, "Hello there. My name is Rose. I don't");
	mvwprintw(talk_to_rose, 2, 1, "have much to say right now.");
	mvwprintw(talk_to_rose, 3, 1, "*Leave*");
	mvwprintw(talk_to_rose, 4, 1, "Why are you here?");
	int talk_to_rose_choices[2] = {3,4};
	int talk_to_rose_choices_size = 2;

	WINDOW *ask_rose_a = newwin(dialog_height, dialog_width, y/2 - dialog_height/2, x/2 - dialog_width/2);
	for(int i=0; i<dialog_width; i++) {
		mvwaddch(ask_rose_a, 0, i, '=');
		mvwaddch(ask_rose_a, 5, i, '=');
	}
	mvwprintw(ask_rose_a, 1, 1, "Well I wanted to adventure.");
	mvwprintw(ask_rose_a, 2, 1, "Too long sitting at home waiting");
	mvwprintw(ask_rose_a, 3, 1, "for adventure to come to me...");
	mvwprintw(ask_rose_a, 4, 1, "<continue>");
	int ask_rose_a_choices[1] = {4};
	int ask_rose_a_choices_size = 1;

	WINDOW *ask_rose_b = newwin(dialog_height, dialog_width, y/2 - dialog_height/2, x/2 - dialog_width/2);
	for(int i=0; i<dialog_width; i++) {
		mvwaddch(ask_rose_b, 0, i, '=');
		mvwaddch(ask_rose_b, 5, i, '=');
	}
	mvwprintw(ask_rose_b, 1, 1, "But it's awfully scary down there.");
	mvwprintw(ask_rose_b, 2, 1, "Could you bring back the treasure");
	mvwprintw(ask_rose_b, 3, 1, "for me? Please?");
	mvwprintw(ask_rose_b, 4, 1, "<exit>");
	int ask_rose_b_choices[1] = {4};
	int ask_rose_b_choices_size = 1;
	
	WINDOW *rose_success = newwin(dialog_height, dialog_width, y/2 - dialog_height/2, x/2 - dialog_width/2);
	for(int i=0; i<dialog_width; i++) {
		mvwaddch(rose_success, 0, i, '=');
		mvwaddch(rose_success, 5, i, '=');
	}
	mvwprintw(rose_success, 1, 1, "Oh wow, you did it!");
	mvwprintw(rose_success, 2, 1, "You're my hero <3");
	mvwprintw(rose_success, 4, 1, "<exit>");
	int rose_success_choices[1] = {4};
	int rose_success_choices_size = 1;

	int obtained_treasure = 0;

	mvwaddch(map, 9, 25, '@'); //rose
	
	//input loop
	char in;
	do {


		if( dialog_level > 0 ) { //dialogs!
			//refresh the screen
			di.refresh_win();

			//player input & handling
			in = wgetch(di.get_win());
			switch(in) {
			case 'w':
				di.move_up();
				break;
			case 's':
				di.move_down();
				break;
			case ' ':
			case '\n':
				switch(dialog_level) {
				case 1: //rose
					if(talk_to_rose_choices[0] == di.select()) {
						di.pop_win();
						dialog_level = 0;
					} else if(talk_to_rose_choices[1] == di.select()) {
						di.add_win(ask_rose_a, ask_rose_a_choices, ask_rose_a_choices_size);
						dialog_level = 2;
					}
					break;
				case 2: //rose
					if(ask_rose_a_choices[0] == di.select()) {
						di.add_win(ask_rose_b, ask_rose_b_choices, ask_rose_b_choices_size);
						dialog_level = 3;
					}
					break;
				case 3: //rose
					if(ask_rose_b_choices[0] == di.select()) {
						di.pop_win(); //2
						di.pop_win(); //1
						di.pop_win(); //0
						dialog_level = 0;
					}
					break;
				case 4: //rose
					if(rose_success_choices[0] == di.select()) {
						di.pop_win();
						dialog_level = 0;
					}
					break;
				}
				break;
			}
		} else { //free movement
			//refresh the screen
			
			debug(log_win,1,std::to_string( enemies.count() ));
			
			for(int i=1; i<(x-log_width)-2; i++) {
				mvwaddch(hp_win,1,i,' ');
			}

			mvwprintw(hp_win,1,1,roman(pc.hp).c_str());

			if(mvwinch(dan.getgrid(),pc.ypos,pc.xpos) == '$') {
				wprintw(log_win,"\nYou see a dollar sign on the floor. Press 'g' to pick it up.");
			}
			
			enemies.draw(map);

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
					obtained_treasure = 1;
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
				switch( move_player(log_win, map, dan.getgrid(), &pc, pc.ypos-1, pc.xpos) ) {
				case 0:
					if(ypos>0 && pc.ypos+1 -ypos +map_start_y == map_size_y/2) ypos --;
					break;
				case 1:
					//wprintw(log_win,"\nYou bonk into the wall!");
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
				case 3:
					if(obtained_treasure == 1) {
						di.add_win(rose_success, rose_success_choices, rose_success_choices_size);
						dialog_level = 4;
					} else {
						di.add_win(talk_to_rose, talk_to_rose_choices, talk_to_rose_choices_size);
						dialog_level = 1;
					}
					break;
				case 4:
				case 5:
					//i don't care about rice here
					break;
				default:
					debug(log_win,99,"Something broke...");
				}
				break;
			case 's':
				switch( move_player(log_win, map, dan.getgrid(), &pc, pc.ypos+1, pc.xpos) ) {
				case 0:
					if(ypos+map_size_y < dan.gety() && pc.ypos-1 -ypos +map_start_y == map_size_y/2) ypos ++;
					break;
				case 1:
					//wprintw(log_win,"\nYou bonk into the wall!");
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
				case 3:
					if(obtained_treasure == 1) {
						di.add_win(rose_success, rose_success_choices, rose_success_choices_size);
						dialog_level = 4;
					} else {
						di.add_win(talk_to_rose, talk_to_rose_choices, talk_to_rose_choices_size);
						dialog_level = 1;
					}
					break;
				case 4:
				case 5:
					//rice
					break;
				default:
					debug(log_win,99,"Something broke...");
				}
				break;
			case 'a':
				switch( move_player(log_win, map, dan.getgrid(), &pc, pc.ypos, pc.xpos-1) ) {
				case 0:
					if(xpos>0 && pc.xpos+1 -xpos +map_start_x == map_size_x/2) xpos --;
					break;
				case 1:
					//wprintw(log_win,"\nYou bonk into the wall!");
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
				case 3:
					if(obtained_treasure == 1) {
						di.add_win(rose_success, rose_success_choices, rose_success_choices_size);
						dialog_level = 4;
					} else {
						di.add_win(talk_to_rose, talk_to_rose_choices, talk_to_rose_choices_size);
						dialog_level = 1;
					}
					break;
				case 4:
				case 5:
					//rice
					break;
				default:
					debug(log_win,99,"Something broke...");
				}
				break;
			case 'd':
				switch( move_player(log_win, map, dan.getgrid(), &pc, pc.ypos, pc.xpos+1) ) {
				case 0:
					if(xpos+map_size_x < dan.getx() && pc.xpos-1 -xpos +map_start_x == map_size_x/2) xpos ++;
					break;
				case 1:
					//wprintw(log_win,"\nYou bonk into the wall!");
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
				case 3:
					if(obtained_treasure == 1) {
						di.add_win(rose_success, rose_success_choices, rose_success_choices_size);
						dialog_level = 4;
					} else {
						di.add_win(talk_to_rose, talk_to_rose_choices, talk_to_rose_choices_size);
						dialog_level = 1;
					}
					break;
				case 4:
				case 5:
					//rice
					break;
				default:
					debug(log_win,99,"Something broke...");
				}
				break;
			}
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
							if( enemies.add_unit(map, 7, 7+rand()%5, 37+rand()%9) == 0) {
								curr = curr->next;
								break;
							}
						}
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
