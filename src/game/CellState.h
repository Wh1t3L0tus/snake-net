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
    SNAKE_4_H = 2048,
    SNAKE_5 = 4096,
    SNAKE_5_H = 8192,
    SNAKE_6 = 16384,
    SNAKE_6_H = 32768,
    SNAKE_7 = 65536,
    SNAKE_7_H = 131072,
    SNAKE_8 = 262144,
    SNAKE_8_H = 524288
};

#endif