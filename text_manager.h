#ifndef __TEXT_MANAGER__
#define __TEXT_MANAGER__

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define BUFF_SIZE 1024

extern unsigned long long text_characters;
extern char* snum;

typedef struct win_info {
	unsigned int mistakes;
	unsigned int typed_so_far;
	float accuracy;
} info_t;

void create_main_window(void);
void print_text_to_win(WINDOW*, FILE*);
void destroy_window(WINDOW*);
unsigned int equal(char, char);
void parse_user_input(WINDOW*, WINDOW*, FILE*, info_t*);
void update_window_attribute(WINDOW*, int);
void starting_screen(WINDOW*, int*);
void blink_text(WINDOW*, int*);
bool kbhit(void);
WINDOW* create_info_window(info_t**);
void update_info_window(WINDOW*, info_t*);
void init_info(info_t*);
unsigned int get_digits_number(void);
void print_stats(WINDOW*, double, float);

#endif
