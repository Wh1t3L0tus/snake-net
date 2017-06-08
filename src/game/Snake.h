#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include <SFML\Graphics.hpp>
#include <list>

#include "Direction.h"

class Snake {

public:

	Snake(sf::Vector2i startPosition, Direction startDirection, int startSize);

	// Called by GameState
	//void MoveTowards(Direction dir);

	int growCounter;
	bool isDead;

	std::list<sf::Vector2i> bodyPositions;

	Direction GetDirection() const;
	void SetDirection(Direction newDirection);

private:

	Direction currentDirection;

	Direction GetOppositeDirection(Direction dir) const;

};

#endif // !SNAKE_H_INCLUDED

