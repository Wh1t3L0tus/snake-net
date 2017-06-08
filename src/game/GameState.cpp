#include <iostream>

#include "GameState.h"

void GameState::Initialize(GameSettings gameSettings)
{
	// create map
	width = 10;
	height = 10;
	map = std::vector<CellState>(width * height, EMPTY);

	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++) 
		{
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1)
			{
				map[j * width + i] = WALL;
			}
		}
	}

	// créer les snakes et initialiser la LookUpTable
	for (size_t s = 0; s < gameSettings.playerCount; s++)
	{
		snakes.push_back(Snake(sf::Vector2i(2, 2 + s), RIGHT, 3));

		snakeLUT.emplace(s, SNAKE_0 << (2 * s));

		AddBit(CellAt(snakes[s].bodyPositions.front), GetHeadBit(snakeLUT[s]));
	}
}

std::vector<CellState> GameState::Update(InputList inputsList)
{
	for (int s = 0; s < inputsList.nbInput; s++)
	{
		snakes[s].SetDirection(inputsList.inputs[s]);

		MakeSnakeMove(snakes[s], snakeLUT[s]);
	}

	ResolveCollisions();

	CleanMapFromDeadSnakes();

	return map;
}

void GameState::MakeSnakeMove(Snake snake, CellState id)
{
	// make snake head move in his direction, 
	// (that have already been set) 
	// then move tail if not growing

	if (snake.isDead)
	{
		return;
	}

	sf::Vector2i currentHeadPos = snake.bodyPositions.front;

	sf::Vector2i newHeadPos = currentHeadPos;
	switch (snake.GetDirection())
	{
		case UP:
			newHeadPos.y -= 1;
			break;
		case DOWN:
			newHeadPos.y += 1;
			break;
		case RIGHT:
			newHeadPos.x += 1;
			break;
		case LEFT:
			newHeadPos.x -= 1;
			break;
		default:
			std::cerr << "A snake shouldn't have NONE direction." << std::endl;
			break;
	}

	RemoveBit(CellAt(currentHeadPos), GetHeadBit(id)); // Remove old head 
	AddBit(CellAt(currentHeadPos), id); // Replace it by body

	AddBit(CellAt(newHeadPos), GetHeadBit(id)); // Add head at new pos

	snake.bodyPositions.push_front(newHeadPos);
	
	if (snake.growCounter <= 0)
	{
		sf::Vector2i tailPos = snake.bodyPositions.back(); 
		snake.bodyPositions.pop_back();

		RemoveBit(CellAt(tailPos), id); // Remove tail
	}
	else 
	{
		snake.growCounter--;
	}
}

void GameState::ResolveCollisions()
{
	// TODO : APPLE MANAGEMENT

	CellState curr; // current cell state 

	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++)
		{
			curr = CellAt(i, j);
			
			for (size_t s = 0; s < snakes.size(); s++)
			{
				CellState snakeHeadValue = GetHeadBit(snakeLUT[s]);

				if ((curr & snakeHeadValue == snakeHeadValue) && (curr != snakeHeadValue))
				{
					snakes[s].isDead = true;
				}
			}
		}
	}
}

void GameState::CleanMapFromDeadSnakes()
{
	for (int s = 0; s < snakes.size(); s++)
	{
		if (snakes[s].isDead)
		{
			while (!snakes[s].bodyPositions.empty())
			{
				sf::Vector2i pos = snakes[s].bodyPositions.front;
				snakes[s].bodyPositions.pop_front();

				// We could optim checking the head only once
				RemoveBit(CellAt(pos), GetHeadBit(snakeLUT[s]));
				RemoveBit(CellAt(pos), snakeLUT[s]);
			}
		}
	}
}

CellState & GameState::CellAt(sf::Vector2i coord)
{
	return map[coord.y * width + coord.x];
}

CellState & GameState::CellAt(int x, int y)
{
	return map[y * width + x];
}

CellState GameState::AddBit(CellState state, CellState bit) const
{
	return static_cast<CellState>(state | bit);
}

CellState GameState::RemoveBit(CellState state, CellState bit) const
{
	return static_cast<CellState>(state & ~bit);
}

CellState GameState::GetHeadBit(CellState bit) const 
{
	return static_cast<CellState>(bit << 1);
}