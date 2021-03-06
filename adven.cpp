#include "adven.h"

void debug(WINDOW *win,int level,std::string input) {
	if(DEBUGGING && level >= DEBUGGING_LEVEL) {
		wprintw(win,"\ndebug:%s",input.c_str());
	}
}

void char_movement(game_state *game, int ydes, int xdes) {
	switch( move_player(game->map, game->dan.getgrid(), &game->pc, ydes, xdes) ) {
	case 0:
		//gotta move camera ... if(ypos>0 && pc.ypos+1 -ypos +map_start_y == map_size_y/2) ypos --;
		break;
	case 1:
		wprintw(game->log_win,"\nYou bonk into the wall!");
		break;
	case 2:
		switch( attack(game->log_win, &game->pc, game->enemies.find_unit(ydes, xdes) ) ) {
		case 0:
			break;
		case 1:
			//game over!
			break;
		case 2:
			game->enemies.delete_unit(ydes, xdes);
			mvwaddch(game->map, ydes, xdes, mvwinch(game->dan.getgrid(), ydes, xdes) );
			break;
		default:
			debug(game->log_win,99,"Something broke!");
		}
		break;
	case 3:
		/*
		if(obtained_treasure == 1) { //no no no dialog stuff here aaaaa
			di.add_win(rose_success, rose_success_choices, rose_success_choices_size);
			dialog_level = 4;
		} else {
			di.add_win(talk_to_rose, talk_to_rose_choices, talk_to_rose_choices_size);
			dialog_level = 1;
		}
		break;
		*/
	default:
		debug(game->log_win,99,"Something broke...");
	}
}

int alpha() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	srand(0); //seed random generator

	int y,x;
	getmaxyx(stdscr,y,x);

	game_state gs;
	gs.ypos = 0;
	gs.xpos = 0;

	gs.pc.name = "player";
	gs.pc.sym = '@';
	gs.pc.ypos = 2;
	gs.pc.xpos = 2;
	gs.pc.hp = 100;
	gs.pc.dmg = 1;

	gs.enemies = unit_list();


	int log_width = 40, hp_height = 3; //size of the hud

	int map_start_y = hp_height, map_start_x = 0, map_size_y = y-hp_height, map_size_x = x-log_width; //start and end points for drawing the map

	gs.hp_win = newwin(hp_height,x-log_width,0,0);
	gs.log_win = newwin(y,log_width,0,x-log_width);

	box(gs.hp_win,0,0);
	box(gs.log_win,0,0);

	scrollok(gs.log_win,true);
	/*
	scrolling doesn't play nice with a border it scrolls the border too
	idk what people do about that whether it's making a new window inside the border
	or just like implementing scrolling themselves which seems like too much trouble
	*/

	gs.map = gs.dan.load_map(FILENAME);

	mvwaddch(gs.map,gs.pc.ypos,gs.pc.xpos,gs.pc.sym);

	wprintw(gs.log_win,"Welcome to Jason's roguelike, <adven.cpp>!");

	refresh();

	//dialog windows here

	dialog di = dialog();

	int dialog_level = 0; //1 or greater when a dialog is open
	

	gs.obtained_treasure = 0;

	mvwaddch(gs.map, 9, 25, '@'); //rose

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

			debug(gs.log_win,1,std::to_string( gs.enemies.count() ));

			for(int i=1; i<(x-log_width)-2; i++) {
				mvwaddch(gs.hp_win,1,i,' ');
			}

			mvwprintw(gs.hp_win,1,1,roman(gs.pc.hp).c_str());

			if(mvwinch(gs.dan.getgrid(),gs.pc.ypos,gs.pc.xpos) == '$') {
				wprintw(gs.log_win,"\nYou see a dollar sign on the floor. Press 'g' to pick it up.");
			}

			gs.enemies.draw(gs.map);

			prefresh(gs.map,gs.ypos,gs.xpos,map_start_y,map_start_x,map_start_y+map_size_y-1,map_start_x+map_size_x-1);
			wrefresh(gs.log_win);
			wrefresh(gs.hp_win);


			//player input & handling
			in = getch();
			switch(in) {
			//pickup
			case 'g':
				if(mvwinch(gs.dan.getgrid(),gs.pc.ypos,gs.pc.xpos) == '$') {
					//inventory management..?
					gs.obtained_treasure = 1;
					mvwaddch(gs.dan.getgrid(),gs.pc.ypos,gs.pc.xpos,'.');
					wprintw(gs.log_win,"\nYou grab the dollar sign!");
				} else {
					wprintw(gs.log_win,"\nNothing here...");
				}
				break;
			//log testing
			case 'l':
				wprintw(gs.log_win,"\nHello there");
				break;
			case 'k':
				wprintw(gs.log_win,"\nGeneral Kenobi");
				break;
			//char movement down here
			case 'w':
				char_movement(&gs, gs.pc.ypos-1, gs.pc.xpos);
				break;
			case 's':
				char_movement(&gs, gs.pc.ypos+1, gs.pc.xpos);
				break;
			case 'a':
				char_movement(&gs, gs.pc.ypos, gs.pc.xpos-1);
				break;
			case 'd':
				char_movement(&gs, gs.pc.ypos, gs.pc.xpos+1);
				break;
			}
		}

		//enemy action
		debug(gs.log_win,0,"enemy action");
		unit_node *curr = gs.enemies.gethead();
		if( curr == NULL ) {
			//no enemies, high chance of spawn
			debug(gs.log_win,0,"no enemies");
			if( rand()%4 == 0 ) {
				debug(gs.log_win,0,"spawn first enemy");
				//position is monster room of testing_map.dat
				for(int i=0; i<SPAWN_ATTEMPTS; i++) {
					if( gs.enemies.add_unit(gs.map, 7, 7+rand()%5, 37+rand()%9) == 0) break;
				}
			}
		} else {
			debug(gs.log_win,0,"some enemies");
			while( curr != NULL ) {
				//move enemy
				debug(gs.log_win,0,"moving an enemy");
				switch(rand()%4) {
				case 0:
					move_enemy(gs.map, gs.dan.getgrid(), &curr->data, curr->data.ypos-1, curr->data.xpos);
					break;
				case 2:
					move_enemy(gs.map, gs.dan.getgrid(), &curr->data, curr->data.ypos+1, curr->data.xpos);
					break;
				case 3:
					move_enemy(gs.map, gs.dan.getgrid(), &curr->data, curr->data.ypos, curr->data.xpos-1);
					break;
				case 1:
					move_enemy(gs.map, gs.dan.getgrid(), &curr->data, curr->data.ypos, curr->data.xpos+1);
					break;
				}

				if( curr->next == NULL ) {
					debug(gs.log_win,0,"spawn a new enemy?");
					//chance to spawn new enemy
					if( rand()%20 == 0 ) {
						debug(gs.log_win,0,"spawn another enemy");
						//position is monster room of testing_map.dat
						for(int i=0; i<SPAWN_ATTEMPTS; i++) {
							if( gs.enemies.add_unit(gs.map, 7, 7+rand()%5, 37+rand()%9) == 0) {
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
