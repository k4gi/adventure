#ifndef DEBUG_H
#define DEBUG_H

#include <ncurses.h>
#include <string>

#define DEBUGGING true
#define DEBUGGING_LEVEL 2

void debug(WINDOW *win,int level,std::string input);

#endif
