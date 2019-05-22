#ifndef UNIT_H
#define UNIT_H

#include <ncurses.h>

struct unit {
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

#endif
