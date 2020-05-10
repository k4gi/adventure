#include "getstring.h"


std::string getString(WINDOW *display) {
	std::string input;

	nocbreak();
	echo();
	curs_set(1);

	int ch = mvwgetch(display,1,1);

	while( ch != '\n' ) {
		input.push_back( ch );
		ch = wgetch(display);
	}

	cbreak();
	noecho();
	curs_set(0);

	return input;
}


