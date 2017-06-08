#include "Snake.h"

#include <iostream>

Snake::Snake(sf::Vector2i startPosition, Direction startDirection, int startSize)
{
	bodyPositions.push_front(startPosition);
	currentDirection = startDirection;
	growCounter = startSize - 1;

	isDead = false;
}

Direction Snake::GetDirection() const
{
	return currentDirection;
}

void Snake::SetDirection(Direction newDirection)
{
	if (newDirection != GetOppositeDirection(currentDirection) && newDirection != NONE)
	{
		currentDirection = newDirection;
	}
}

Direction Snake::GetOppositeDirection(Direction dir) const
{
	switch (dir)
	{
		case UP:
			return DOWN;
		case DOWN:
			return UP;
		case RIGHT:
			return LEFT;
		case LEFT:
			return RIGHT;
		default: // NONE
			return NONE;
	}
}
