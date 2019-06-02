#include "dialog.h"

dialog_selector *dialog::add_selector(int choices[], int choices_size) {
	dialog_selector *new_selector = new dialog_selector;
	new_selector->line = choices[0];
	new_selector->prev = new_selector;
	new_selector->next = new_selector;
	
	dialog_selector *curr;
	dialog_selector *prev = new_selector;
	for(int i=1;i<choices_size;i++) {
		curr = new dialog_selector;
		curr->line = choices[i];
		curr->prev = prev;
		curr->next = new_selector;
		prev->next = curr;
		prev = curr;
	}
	new_selector->prev = curr;

	return new_selector;
}


dialog::dialog(WINDOW *win, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;
	new_node->data = win;
	new_node->prev = NULL;
	new_node->selector = add_selector(choices, choices_size);
	top = new_node;
}

void dialog::add_win(WINDOW *win, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;
	new_node->data = win;
	new_node->prev = top;
	new_node->selector = add_selector(choices, choices_size);
	top = new_node;
}

void dialog::pop_win() {
	if(top != NULL) {
		dialog_win *curr = top;
		top = top->prev;
		free(curr);
	}
}

int dialog::select() {
	return top->selector->line;
}

void dialog::move_up() {
	top->selector = top->selector->prev;
}

void dialog::move_down() {
	top->selector = top->selector->next;
}

void dialog::refresh_win() {
	int y,x;
	getmaxyx(top->data,y,x);

	for(int i=0; i<y; i++) {
		//chtype *thisline = (chtype *) malloc( sizeof(chtype)*x );
		//mvwinchstr(top->data, i, 0, thisline);
		char *thisline = (char *) malloc( sizeof(char)*(x+1) );
		mvwinstr(top->data, i, 0, thisline);
		if(i != top->selector->line) {
			wattroff(top->data, A_REVERSE);
		} else {
			wattron(top->data, A_REVERSE);
		}
		//mvwaddchstr(top->data, i, 0, thisline);
		mvwaddstr(top->data, i, 0, thisline);
		free(thisline);
	}
	wattrset(top->data, A_NORMAL);

	wrefresh(top->data);
}

WINDOW *dialog::get_win() {
	return top->data;
}
