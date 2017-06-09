#ifndef CELLSTATE_H
#define CELLSTATE_H

enum CellState {
	EMPTY = 0,
	WALL = 1,
	APPLE = 2,
	SNAKE_0 = 4,
	SNAKE_0_H = 8,
	SNAKE_1 = 16,
	SNAKE_1_H = 32,
	SNAKE_2 = 64,
	SNAKE_2_H = 128,
	SNAKE_3 = 256,
	SNAKE_3_H = 512,
	SNAKE_4 = 1024,
	SNAKE_4_H = 2048
};

#endif