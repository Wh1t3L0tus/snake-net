#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>

enum CellState {
	EMPTY = 0,
	WALL = 1,
	SNAKE_1 = 2,
	SNAKE_2 = 4,
	SNAKE_3 = 8,
	SNAKE_4 = 16,
	SNAKE_5 = 32,
	SNAKE_6 = 64,
	SNAKE_7 = 128
};

class GameState {

	


private:

	int width;
	int height;

	std::vector<CellState> map;
};

#endif // 
