all: grid

grid: grid.c
	$(CC) grid.c -o grid -Wall -g -Wextra -pedantic -std=c99 -lm
