#include "unit.h"

int move_player(WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos) {
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '#':
		//wall
		return 1;
	case '7':
		//attack
		return 2;
	default:
		mvwaddch(map, player->ypos, player->xpos, mvwinch(grid, player->ypos, player->xpos) );
		player->ypos = t_ypos;
		player->xpos = t_xpos;
		mvwaddch(map, player->ypos, player->xpos, player->sym);
		return 0;
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

unit *find_unit(unit_node *list, int ypos, int xpos) {
	unit_node *curr = list;
	while( curr != NULL ) {
		if(curr->data.ypos == ypos && curr->data.xpos == xpos) return &curr->data;
		curr = curr->next;
	}
	return NULL;
}

void delete_unit(unit_node *list, int ypos, int xpos) {
	if( list == NULL ) {
		return;
	} else if( list->data.ypos == ypos && list->data.xpos == xpos ) {
		unit_node *curr = list;
		list = list->next;
		free(curr);
		return;
	}
	unit_node *prev = list;
	unit_node *curr = list->next;
	while( curr != NULL ) {
		if( curr->data.ypos == ypos && curr->data.xpos == xpos ) {
			prev->next = curr->next;
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}
