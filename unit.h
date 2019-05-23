#ifndef UNIT_H
#define UNIT_H

#include <ncurses.h>
#include <string>

struct unit {
	std::string name;
	char sym;
	int ypos;
	int xpos;
	int hp;
	int dmg;
};

struct unit_node {
	unit data;
	unit_node *next;
};

/*
int move_player returns
0 for successful movement
1 for no movement
2 for attack action
*/
int move_player(WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos);

/*
int attack returns
0 for inconclusive round
1 for attacker losing
2 for defender losing
*/
int attack(WINDOW *log, unit *attacker, unit *defender);

unit *find_unit(unit_node *list, int ypos, int xpos);

#endif
