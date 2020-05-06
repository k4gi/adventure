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

dialog::dialog() {
	top = NULL;
}

dialog::dialog(WINDOW *win, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;
	new_node->data = win;
	new_node->prev = NULL;
	new_node->selector = add_selector(choices, choices_size);
	top = new_node;
}

dialog::dialog(int height, int width, std::string lines[], int lines_size, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;

	WINDOW *new_win = newwin(height, width, 10, 10);
	for(int i=0; i<width; i++) {
		mvwaddch(new_win, 0, i, '=');
		mvwaddch(new_win, 5, i, '=');
	}
	for(int i=0; i<height && i<lines_size; i++) {
		mvwprintw(new_win, i+1, 1, lines[i].c_str());
	}

	new_node->data = new_win;
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

void dialog::add_win(int height, int width, std::string lines[], int lines_size, int choices[], int choices_size) {
	dialog_win *new_node = new dialog_win;

	WINDOW *new_win = newwin(height, width, 10, 10);
	for(int i=0; i<width; i++) {
		mvwaddch(new_win, 0, i, '=');
		mvwaddch(new_win, 5, i, '=');
	}
	for(int i=0; i<height && i<lines_size; i++) {
		mvwprintw(new_win, i+1, 1, lines[i].c_str());
	}

	new_node->data = new_win;
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
	if(top != NULL) {
		return top->selector->line;
	} else {
		return -1;
	}
}

void dialog::move_up() {
	if(top != NULL) {
		top->selector = top->selector->prev;
	}
}

void dialog::move_down() {
	if(top != NULL) {
		top->selector = top->selector->next;
	}
}

void dialog::refresh_win() {
	if(top != NULL) {
		int y,x;
		getmaxyx(top->data,y,x);

		for(int i=0; i<y; i++) {
			//chtype *thisline = (chtype *) malloc( sizeof(chtype)*x );
			//mvwinchstr(top->data, i, 0, thisline);
			char *thisline = (char *) malloc( sizeof(char)*(x+1) );
			mvwinstr(top->data, i, 0, thisline);
			if(i != top->selector->line) {
				wattroff(top->data, HIGHLIGHTING);
			} else {
				wattron(top->data, HIGHLIGHTING);
			}
			//mvwaddchstr(top->data, i, 0, thisline);
			mvwaddstr(top->data, i, 0, thisline);
			free(thisline);
		}
		wattrset(top->data, A_NORMAL);

		wrefresh(top->data);
	}
}

WINDOW *dialog::get_win() {
	if(top != NULL) {
		return top->data;
	} else {
		return NULL;
	}
}

void dialog::decorate(char l, char r, char t, char b, char tl, char tr, char bl, char br) {
	wborder(top->data, l, r, t, b, tl, tr, bl, br);
}
