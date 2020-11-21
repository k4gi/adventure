#include "unit.h"

//move_player should have sooo much more stuff happening in here, rather than passing return values straight to adven.cpp
int move_player(WINDOW *log, WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos) {
	bool emptychest = false;
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '#':
		//wall
		wprintw(log,"\nYou stub your toe on the wall.");
		return 1;
	case '7':
		//attack
		return 2;
	case '@':
		//chat
		//oh so the code for the 'dialog system' should run in here.
		//a function that hands over the t_ypos and t_xpos
		return 3;
	case '&':
		//sack of rice
		//uuhhh this seems like a good place to start in adding more content
		wprintw(log,"\nThis is a bag of rice. Do you want to eat from it?\n(press y to eat, anything else to cancel.");
		char inputcharacter;
		do {
			inputcharacter = wgetch(log);
			switch(inputcharacter) {
			case 'y':
				wprintw(log,"\nNom nom. Tasty rice.");
				player->hp = 100;
				return 4;
			default:
				wprintw(log,"\nNo rice for you then.");
				return 5;

			}
		} while( inputcharacter != 'q' );
	case '=':
		//chest
		wprintw(log,"\nYou see a chest.");
		if( !emptychest ) {
			wprintw(log,"\nYou reach in to take something...");
			//would this part of the program know which items are in which chests?
			return 6;
		}
		return 1; // bonk
	default:
		mvwaddch(map, player->ypos, player->xpos, mvwinch(grid, player->ypos, player->xpos) );
		player->ypos = t_ypos;
		player->xpos = t_xpos;
		mvwaddch(map, player->ypos, player->xpos, player->sym);
		return 0;
	}
}

int move_enemy(WINDOW *map, WINDOW *grid, unit *enemy, int t_ypos, int t_xpos) {
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '#':
	case '+':
	case '7':
		//wall
		return 1;
	case '@':
		//attack
		return 2;
	default:
		mvwaddch(map, enemy->ypos, enemy->xpos, mvwinch(grid, enemy->ypos, enemy->xpos) );
		enemy->ypos = t_ypos;
		enemy->xpos = t_xpos;
		mvwaddch(map, enemy->ypos, enemy->xpos, enemy->sym);
		return 0;
	}
}

//just like, popping this here. If I want to work out how to make changes, I . . . need to try and make changes.
int move_creep(WINDOW *map, WINDOW *grid, unit *creep) {
	switch( mvwinch(grid, creep->ypos, creep->xpos) ) {
	case '>': //move right
		switch( mvwinch(map, creep->ypos, creep->xpos+1) ) {
		case '>':
		case 'v':
		case '<':
		case '^':
		case 'x':
			//it's ok to move bc we're not going off the track
			mvwaddch(map, creep->ypos, creep->xpos, mvwinch(grid, creep->ypos, creep->xpos) );
			creep->xpos += 1;
			mvwaddch(map, creep->ypos, creep->xpos, creep->sym);
			return 3;
		default:
			//you fool you've run off the track
			//where's my goddamn debug function
			//sigh
			return 1;
		}
		break;
	case 'v': //move down
		switch( mvwinch(map, creep->ypos+1, creep->xpos) ) {
		case '>':
		case 'v':
		case '<':
		case '^':
		case 'x':
			//it's ok to move bc we're not going off the track
			mvwaddch(map, creep->ypos, creep->xpos, mvwinch(grid, creep->ypos, creep->xpos) );
			creep->ypos += 1;
			mvwaddch(map, creep->ypos, creep->xpos, creep->sym);
			return 6;
		default:
			//you fool you've run off the track
			//where's my goddamn debug function
			//sigh
			return 1;
		}
		break;
	case '<': //move left
		switch( mvwinch(map, creep->ypos, creep->xpos-1) ) {
		case '>':
		case 'v':
		case '<':
		case '^':
		case 'x':
			//it's ok to move bc we're not going off the track
			mvwaddch(map, creep->ypos, creep->xpos, mvwinch(grid, creep->ypos, creep->xpos) );
			creep->xpos -= 1;
			mvwaddch(map, creep->ypos, creep->xpos, creep->sym);
			return 9;
		default:
			//you fool you've run off the track
			//where's my goddamn debug function
			//sigh
			return 1;
		}
		break;
	case '^': //move up
		switch( mvwinch(map, creep->ypos-1, creep->xpos) ) {
		case '>':
		case 'v':
		case '<':
		case '^':
		case 'x':
			//it's ok to move bc we're not going off the track
			mvwaddch(map, creep->ypos, creep->xpos, mvwinch(grid, creep->ypos, creep->xpos) );
			creep->ypos -= 1;
			mvwaddch(map, creep->ypos, creep->xpos, creep->sym);
			return 12;
		default:
			//you fool you've run off the track
			//where's my goddamn debug function
			//sigh
			return 1;
		}
		break;
	case 'x': //destination
		//special one-time offer of moving right to start moving through the test track again
		//ordinarily uhhh... doing damage to the player? idk
		mvwaddch(map, creep->ypos, creep->xpos, mvwinch(grid, creep->ypos, creep->xpos) );
		creep->xpos += 1;
		mvwaddch(map, creep->ypos, creep->xpos, creep->sym);
		return 3;
	default:
		//we're off the track. there's nowhere to go. do nothing.
		return 1;
	}
}

int attack(WINDOW *log, unit *attacker, unit *defender) {
	defender->hp -= attacker->dmg;
	wprintw(log,"\nThe %s strikes for %d, reducing the %s to %d HP!",attacker->name.c_str(),attacker->dmg,defender->name.c_str(),defender->hp);
	if(defender->hp <= 0) {
		wprintw(log,"\nThe %s is defeated!",defender->name.c_str());
		return 2;
	} else {
		attacker->hp -= defender->dmg;
		wprintw(log,"\nThe %s retaliates for %d, reducing the %s to %d HP!",defender->name.c_str(),defender->dmg,attacker->name.c_str(),attacker->hp);
		if(attacker->hp <= 0) {
			wprintw(log,"\nThe %s is defeated!",attacker->name.c_str());
			return 1;
		} else {
			return 0;
		}
	}
}

unit_list::unit_list() {
	head = NULL;
}

int unit_list::add_unit(WINDOW *map, int type, int ypos, int xpos) {
	if( mvwinch(map, ypos, xpos) != '.' ) return 1;
	unit_node *new_enemy = new unit_node;
	new_enemy->data.ypos = ypos;
	new_enemy->data.xpos = xpos;
	new_enemy->next = NULL;

	switch(type) {
	case 7:
	default:
		new_enemy->data.name = "seven";
		new_enemy->data.sym = '7';
		new_enemy->data.hp = 2;
		new_enemy->data.dmg = 1;
	}

	if(head == NULL) {
		head = new_enemy;
	} else {
		unit_node *curr = head;
		while(curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = new_enemy;
	}
	return 0;
}

unit *unit_list::find_unit(int ypos, int xpos) {
	unit_node *curr = head;
	while( curr != NULL ) {
		if(curr->data.ypos == ypos && curr->data.xpos == xpos) return &curr->data;
		curr = curr->next;
	}
	return NULL;
}

int unit_list::delete_unit(int ypos, int xpos) {
	if( head == NULL ) {
		return 2;
	} else if( head->data.ypos == ypos && head->data.xpos == xpos ) {
		unit_node *curr = head;
		head = head->next;
		free(curr);
		return 1;
	}
	unit_node *prev = head;
	unit_node *curr = head->next;
	while( curr != NULL ) {
		if( curr->data.ypos == ypos && curr->data.xpos == xpos ) {
			prev->next = curr->next;
			free(curr);
			return 0;
		}
		prev = curr;
		curr = curr->next;
	}
	return 3;
}

unit_node *unit_list::gethead() {
	return head;
}

void unit_list::draw(WINDOW *map) {
	for( unit_node *curr = head; curr != NULL; curr = curr->next ) {
		mvwaddch(map, curr->data.ypos, curr->data.xpos, curr->data.sym);
	}
}

int unit_list::count() {
	int output = 0;
	unit_node *curr = head;
	while(curr != NULL) {
		output ++;
		curr = curr->next;
	}
	return output;
}
