#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

#include "Snake.h"
#include "..\network\Network.h"
#include "CellState.h"


class GameState {

public:

	bool Initialize(GameSettings gameSettings);
	void Update(InputList inputsList);

	bool IsGameOver() const;
	std::string GetGameOverMessage() const;

	int width;
	int height;

	std::vector<CellState> GetMap() const { return map; }

	const std::vector<Snake>& GetSnakes() const { return snakes; }

private:

	bool isAppleEaten;
	void GenerateApple();

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
