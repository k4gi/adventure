#ifndef UNIT_H
#define UNIT_H

#include <ncurses.h>
#include <string>

#include "debug.h"

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

class unit_list {
	unit_node *head;
public:
	unit_list();
	int add_unit(WINDOW *map, int type, int ypos, int xpos);
	unit *find_unit(int ypos, int xpos);

	/*
	int delete_unit returns
	0 for standard delete
	1 for deleting first
	2 for empty list
	*/
	int delete_unit(int ypos, int xpos);
	unit_node *gethead();
	void draw(WINDOW *map);
	int count();
};

/*
int move_player returns
0 for successful movement
1 for no movement
2 for attack action
3 for chat with npc
4 for eaating rice
5 for not eating rice
6 for taking from chest
*/

int move_player(WINDOW *log, WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos);
int move_enemy(WINDOW *map, WINDOW *grid, unit *enemy, int t_ypos, int t_xpos);
int move_creep(WINDOW *map, WINDOW *grid, unit *creep);
int aim_tower(WINDOW *log, WINDOW *map, WINDOW *grid, unit *tower, unit_list creeps);

/*
int attack returns
0 for inconclusive round
1 for attacker losing
2 for defender losing
*/
int attack(WINDOW *log, unit *attacker, unit *defender);
int aim_tower_attack(WINDOW *log, unit *tower, unit *creep);

#endif
