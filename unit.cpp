#include "unit.h"

int move_player(WINDOW *map, WINDOW *grid, unit *player, int t_ypos, int t_xpos) {
	switch( mvwinch(map, t_ypos, t_xpos) ) {
	case '#':
		//wall
		return 1;
	case '7':
		//attack
		return 2;
	default:
		mvwaddch(map, player->ypos, player->xpos, mvwinch(grid, player->ypos, player->xpos) );
		player->ypos = t_ypos;
		player->xpos = t_xpos;
		mvwaddch(map, player->ypos, player->xpos, player->sym);
		return 0;
	}
}
