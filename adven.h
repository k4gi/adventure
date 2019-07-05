#ifndef ADVEN_H
#define ADVEN_H

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

struct game_state {
	int ypos;
	int xpos; //top left corner of the visible map
	unit pc;
	unit_list enemies;
	map_loader dan;
	WINDOW *map;
	WINDOW *hp_win;
	WINDOW *log_win;
	int obtained_treasure;
};

void debug(WINDOW *win,int level,std::string input);
int main();

#endif
