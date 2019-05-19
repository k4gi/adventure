#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream> //read & write
#include <ncurses.h>
#include <string>

class map_loader {
	int pad_y = 0; //number of lines
	int pad_x = 0; //size of string
	//char **grid;
	WINDOW *grid = NULL;
public:
	WINDOW *load_map(std::string filename);
	int getx();
	int gety();
	WINDOW *getgrid();
};

#endif
