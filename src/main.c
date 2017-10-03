#include <stdlib.h>
#include "text_manager.h"

unsigned long long text_characters = 0;
char* snum;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "You must provide more arguments!\n");
    exit(EXIT_FAILURE);
  }
  FILE* fp;
  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    perror("Couldn't open the target file!\n");
    exit(EXIT_FAILURE);
  }
  WINDOW* my_win;
  int blc[4];		/* I name this "blink coordinates" */
  create_main_window();
  my_win = stdscr;

  /* TODO (George): In the case that the terminal doesn't support 
     colors, maybe I should ask the user if they want to play without
     colors and just use text highlighting, or terminate the game     */

  if (has_colors() == TRUE) {
		handle_color_case(my_win);
  } else {
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
  }
  starting_screen(my_win, blc);
  scrollok(my_win, TRUE);
  wrefresh(my_win);
  print_text_to_win(my_win, fp);
  unsigned int digits = get_digits_number();
  snum = (char*) malloc((digits + 1) * sizeof(char));
  sprintf(snum, "%llu", text_characters);
  info_t* info;
  WINDOW* info_win = create_info_window(&info);
  init_info(info);
  update_info_window(info_win, info);
  move(4, 0);
  fseek(fp, 0, SEEK_SET);
	struct timeval t1, t2;
	double elapsed_time;
	gettimeofday(&t1, NULL);
  parse_user_input(my_win, info_win, fp, info);
	gettimeofday(&t2, NULL);
	elapsed_time = (t2.tv_sec - t1.tv_sec);
	curs_set(0);
	print_stats(my_win, elapsed_time, info->accuracy);
	curs_set(1);
  getch();
	destroy_window(my_win);
	destroy_window(info_win);
  fclose(fp);
  free(snum);
  free(info);
  return EXIT_SUCCESS;
}

