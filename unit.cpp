#include "unit.h"

int move_player(WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos) {
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '#':
		//wall
		return 1;
	case '7':
		//attack
		return 2;
	case '@':
		//chat
		return 3;
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

void unit_list::add_unit(int type, int ypos, int xpos) {
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
