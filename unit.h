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
int move_enemy(WINDOW *map, WINDOW *grid, unit *enemy, int t_ypos, int t_xpos);

/*
int attack returns
0 for inconclusive round
1 for attacker losing
2 for defender losing
*/
int attack(WINDOW *log, unit *attacker, unit *defender);

class unit_list {
	unit_node *head;
public:
	unit_list();
	void add_unit(int type, int ypos, int xpos);
	unit *find_unit(int ypos, int xpos);

	/*
	int delete_unit returns
	0 for standard delete
	1 for deleting first
	2 for empty list
	*/
	int delete_unit(int ypos, int xpos);
	unit_node *gethead();
};

#endif
