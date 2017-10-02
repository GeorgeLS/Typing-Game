CC = gcc
FLAGS = -Wall
DEBUG = -ggdb

bin: text_manager.o main.o text_manager.h
	$(CC) $(FLAGS) $(DEBUG) text_manager.o main.o -o typing_game -lncurses

text_manager.o: text_manager.c text_manager.h
	$(CC) $(FLAGS) $(DEBUG) -c text_manager.c -lncurses

main.o: main.c text_manager.h
	$(CC) $(FLAGS) $(DEBUG) -c main.c -lncurses

.PHONY : clear

clear: main.o text_manager.o typing_game
	rm -f main.o text_manager.o typing_game

