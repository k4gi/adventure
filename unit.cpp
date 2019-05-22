#include "unit.h"

int move_player(WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos) {
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '.':
	case '+':
	case '$':
		// oh jesus i gotta move the camera too
		//if(ypos>0 && pc.ypos-ypos+map_start_y == map_size_y/2) ypos --;
		mvwaddch(map, player->ypos, player->xpos, mvwinch(grid, player->ypos, player->xpos) );
		player->ypos = t_ypos;
		player->xpos = t_xpos;
		mvwaddch(map, player->ypos, player->xpos, player->sym);
		return 0;
	case '7':
		//attack
		return 2;
	case '#':
		//wall
		return 1;
	}
}
