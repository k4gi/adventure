#ifndef UNIT_H
#define UNIT_H

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

#endif
