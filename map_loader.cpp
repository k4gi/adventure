#include "map_loader.h"

WINDOW *map_loader::load_map(std::string filename) {
	/*
		how about we open the file once, measure, the map, then close it.
		Then open the file again to read it into a pad.
		It means running through the file twice but then I know what dimensions to make the pad.
		That's good, right?
	*/
	pad_y = pad_x = 0; //reset
	if(grid != NULL) {
		free(grid);
		grid = NULL;
	}

	WINDOW *map_pad; //output 
	std::ifstream loader; //file handler!
	//measuring
	loader.open(filename,std::ios::in);
	if( loader.is_open() ) {
		std::string input_line;

		while(std::getline(loader, input_line)) {
			pad_y ++;
			if(input_line.length() > pad_x) pad_x = input_line.length();
		}
		loader.close();
	}

	map_pad = newpad(pad_y,pad_x);
	//grid[pad_y][pad_x];

	//printing
	loader.open(filename,std::ios::in);
	if( loader.is_open() ) {
		std::string input_line;
		int inc = 0;

		while(std::getline(loader, input_line)) {
			mvwprintw(map_pad,inc,0,input_line.c_str());
			//grid[inc] = input_line.c_str();
			inc ++;
		}
		loader.close();
	}

	grid = dupwin(map_pad);

	return map_pad;
}

int map_loader::gety() {
	return pad_y;
}

int map_loader::getx() {
	return pad_x;
}

WINDOW *map_loader::getgrid() {
	return grid;
}
