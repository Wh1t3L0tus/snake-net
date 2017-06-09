#include "GameState.h"
#include "MapLoader.h"

#include <iostream>

bool GameState::Initialize(GameSettings gameSettings)
{
	srand(gameSettings.seed);

	// create map
	Map loadedMap;
	if (!MapLoader::LoadMap(gameSettings.mapName, loadedMap)) {

		std::cerr << "Cannot load map " << gameSettings.mapName << std::endl;
		return false;
	}

	width = loadedMap.width;
	height = loadedMap.height;
	map = loadedMap.tiles;

	std::cout << "[GameState] Player count : " << (int)gameSettings.playerCount << std::endl;

	// create snakes and initialize look up table
	auto spawnerIterator = loadedMap.spawners.begin();
	for (size_t s = 0; s < gameSettings.playerCount; s++)
	{
		snakes.push_back(Snake(sf::Vector2i(spawnerIterator->coords), spawnerIterator->direction, 3));

		snakeLUT.emplace(s, static_cast<CellState>(SNAKE_0 << (2 * s)));

		AddBit(CellAt(snakes[s].bodyPositions.front()), GetHeadBit(snakeLUT[s]));

		++spawnerIterator;
	}

	isAppleEaten = true;

	return true;
}

void GameState::Update(InputList inputsList)
{
	CleanMapFromDeadSnakes();

	for (int s = 0; s < inputsList.nbInput; s++)
	{
		snakes[s].SetDirection(inputsList.inputs[s]);

		MakeSnakeMove(snakes[s], snakeLUT[s]);
	}

	ResolveCollisions();

	if (isAppleEaten)
	{
		GenerateApple();
	}
}

void GameState::GenerateApple()
{
	while (isAppleEaten)
	{
		int x = (int)(rand() / (float)RAND_MAX * width);
		int y = (int)(rand() / (float)RAND_MAX * height);

		if (CellAt(x, y) == EMPTY)
		{
			AddBit(CellAt(x, y), APPLE);
			isAppleEaten = false;
		}
	}
}

void GameState::MakeSnakeMove(Snake& snake, CellState id)
{
	// make snake head move in his direction, 
	// (that have already been set) 
	// then move tail if not growing

	if (snake.isDead)
	{
		return;
	}

	sf::Vector2i currentHeadPos = snake.bodyPositions.front();

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
	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++)
		{
			CellState& curr = CellAt(i, j);
			
			for (size_t s = 0; s < snakes.size(); s++)
			{
				CellState snakeHeadValue = GetHeadBit(snakeLUT[s]);

				if (((curr & snakeHeadValue) == snakeHeadValue) && (curr != snakeHeadValue))
				{
					if (curr == (snakeHeadValue | APPLE))
					{
						std::cout << "Snake " << s << " have eaten apple :D" << std::endl;
						snakes[s].growCounter += 1;
						RemoveBit(curr, APPLE);
						isAppleEaten = true;
					}
					else 
					{
						std::cout << "Snake " << s << " had been killed. (" << curr << ")" << std::endl;
						snakes[s].isDead = true;
					}
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
				sf::Vector2i pos = snakes[s].bodyPositions.front();
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

void GameState::AddBit(CellState& state, const CellState bit)
{
	state = static_cast<CellState>(state | bit);
}

void GameState::RemoveBit(CellState& state, const CellState bit)
{
	state = static_cast<CellState>(state & ~bit);
}

CellState GameState::GetHeadBit(const CellState bit) const 
{
	return static_cast<CellState>(bit << 1);
}