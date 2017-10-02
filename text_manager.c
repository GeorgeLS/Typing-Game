#include "text_manager.h"

char buffer[BUFF_SIZE];

void create_main_window(void) {
	initscr();
	raw();
	cbreak();
	noecho();
}

/* TODO (George): I have to find a way so that the text wraps around 
	 the sides and doesn't split the words in an ugly way              */

void print_text_to_win(WINDOW* win, FILE* fp) {
	size_t n_read;
	move(4, 0);
	while ((n_read = fread(buffer, sizeof(char),
												 sizeof(buffer), fp)) > 0) {
		wprintw(win, "%s", buffer);
		text_characters += strlen(buffer);
	} --text_characters;
}

void destroy_window(WINDOW* win) {
	if (win == stdscr) endwin();
	else delwin(win);
}

unsigned int equal(char c1, char c2) {
	return (c1 == c2) ? 1 : 0;
}

void parse_user_input(WINDOW* win, WINDOW* info_win, FILE* fp, info_t* info) {
	int fch;
	while ((fch = fgetc(fp)) != EOF) {
		int cch;
		if ((cch = fgetc(fp)) != EOF) ungetc(cch, fp);
		else break;
		int ch = getch();
		if (!equal(ch, fch)) {
			addch(fch | A_STANDOUT | COLOR_PAIR(1));
			++info->mistakes;
		}
		else addch(fch);
		++info->typed_so_far;
		info->accuracy = (info->typed_so_far - info->mistakes)/(float) info->typed_so_far;
		update_info_window(info_win, info);
		wrefresh(win);
	}
}

void starting_screen(WINDOW* win, int* blc) {
	clear();
	curs_set(0);
	int y, x;
	char* str1 = "Welcome to the typing game!!!";
	char* str2 = "Are you ready to find out how fast and accurately you \
can type?";
	char* str3 = "If yes, ";
	char* str4 = "press any button";
	char* str5 = " to start";
	size_t complete = strlen(str3) + strlen(str4) + strlen(str5);
	getmaxyx(win, y, x);
	int midy = y/2;
	mvwprintw(win, midy - 1, (x - strlen(str1))/2, "%s", str1);
	mvwprintw(win, midy, (x - strlen(str2))/2, "%s", str2);
	mvwprintw(win, midy + 1, (x - complete)/2, "%s", str3);
	getyx(win, blc[0], blc[1]);
	wprintw(win, "%s", str4);
	getyx(win, blc[2], blc[3]);
	wprintw(win, "%s", str5);
	refresh();
	blink_text(win, blc);
	clear();
	attron(A_STANDOUT);
	for (int i = 3; i != 0; --i) {
		mvwprintw(win, midy, x/2, "%d", i);
		wrefresh(win);
		napms(600);
	}
	mvwprintw(win, midy, (x - 3)/2, "GO!");
	wrefresh(win);
	napms(400);
	attroff(A_STANDOUT);
	clear();
	curs_set(1);
}

void blink_text(WINDOW* win, int* blc) {
	while (!kbhit()) {
		napms(500);
		if (kbhit()) break;
		wmove(win, blc[0], blc[1]);
		if (kbhit()) break;
		for (int i = blc[1]; i != blc[3]; ++i) {
			if (kbhit()) break;
			waddch(win, ' ');
		}
		if (kbhit()) break;
		wrefresh(win);
		if (kbhit()) break;
		napms(400);
		if (kbhit()) break;
		mvwprintw(win, blc[0], blc[1], "press any button");
		if (kbhit()) break;
		wrefresh(win);
		if (kbhit()) break;
	}
	getch();
}

bool kbhit(void) {
  int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
  return byteswaiting > 0;
}

WINDOW* create_info_window(info_t** info) {
	WINDOW* new = newwin(3, COLS, 0, 0);
	box(new, 0, 0);
	wrefresh(new);
	*info = (info_t*) malloc(sizeof(info_t));
	return new;
}

void update_info_window(WINDOW* win, info_t* info) {
	werase(win);
	box(win, 0, 0);
	mvwprintw(win, 1, 2, "Remaining characters = %lu", text_characters - info->typed_so_far);
	size_t str_size = strlen("Accuracy = ") + 3;
	mvwprintw(win, 1, (COLS - str_size)/2, "Accuracy = %.2f%%", info->accuracy * 100.0);
	str_size = strlen("Total characters  = ") + strlen(snum) + 1;
	mvwprintw(win, 1, (COLS - str_size), "Total Characters = %llu", text_characters);
	wrefresh(win);
}

void init_info(info_t* info) {
	info->accuracy = 0.0F;
	info->typed_so_far = 0U;
	info->mistakes = 0U;
}

unsigned int get_digits_number(void) {
	unsigned long long aux = text_characters;
	unsigned int digits;
	while (aux) {
		++digits;
		aux /= 10;
	}
	return digits;
}

void print_stats(WINDOW* win, double t, float ac) {
	wclear(win);
	attron(A_UNDERLINE);
	mvwprintw(win, LINES/2, (COLS - strlen("STATS"))/2, "STATS");
	attroff(A_UNDERLINE);
	size_t beggining = COLS/2 - strlen("STATS")/2 - 20;
	mvwprintw(win, LINES/2 + 2, beggining, "It took you %.1f seconds to write the text", t);
	mvwprintw(win, LINES/2 + 4, beggining, "You were typing %.1f characters per second", (float) text_characters / t);
	mvwprintw(win, LINES/2 + 6, beggining, "You were accurate by %.2f%%", ac * 100.0);
	wrefresh(win);
}
