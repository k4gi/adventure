#include "dialog.h"

dialog::dialog(WINDOW *win, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;
	new_node->data = win;
	for(int i=0;i<choices_size;i++) {
		//hmm
	new_node->selector //AAA
	new_node->selected = 0;
	new_node->prev = NULL;
	top = new_node;
}

void dialog::add_win(WINDOW *win, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;
	new_node->data = win;
	new_node->selector //AAA
	new_node->selected = 0;
	new_node->prev = top;
	top = new_node;
}

void dialog::pop_win() {
	if(top != NULL) {
		dialog_win curr = top;
		top = top->prev;
		free(curr);
	}
}

int dialog::select() {
	return top->choices[top->selected];
}

	void move_up();
	void move_down();
WINDOW *dialog::get_win() {
	return top->data;
}
