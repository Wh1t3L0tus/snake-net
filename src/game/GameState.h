#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

#include "Snake.h"
#include "..\network\Network.h"

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

class GameState {

public:

	void Initialize(GameSettings gameSettings);
	void Update(InputList inputsList);

	bool IsGameOver() const;
	std::string GetGameOverMessage() const;

	int width;
	int height;

	std::vector<CellState> GetMap() const { return map; }

private:


	std::vector<CellState> map;
	std::vector<Snake> snakes;

	// snake ID ; corresponding enum
	std::map<int, CellState> snakeLUT;

	void MakeSnakeMove(Snake& snake, CellState id);
	void ResolveCollisions();
	void CleanMapFromDeadSnakes();

	CellState& CellAt(sf::Vector2i);
	CellState& CellAt(int x, int y);

	void AddBit(CellState& state, const CellState bit);
	void RemoveBit(CellState& state, const CellState bit);
	CellState GetHeadBit(const CellState bit) const;
};

#endif // GAME_STATE_H
