#include "debug.h"

void debug(WINDOW *win,int level,std::string input) {
        if(DEBUGGING && level >= DEBUGGING_LEVEL) {
                wprintw(win,"\ndebug:%s",input.c_str());
        }
}


