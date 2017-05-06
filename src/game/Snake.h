#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include <SFML\Graphics.hpp>
#include <list>

#include "Direction.h"

/**
 * Describe the move behavior of our snakes and how to draw them on screen
 */
class Snake : public sf::Drawable {

public:

	/** Constructor
	 * 
	 * @param head : sprite to use to display snake's head
	 * @param body : sprite to use to display snake's body part
	 * @param pos : position of the snake's head at instanciation
	 * @param dir : initial direction of the snake
	 */
	Snake(sf::Sprite& head, sf::Sprite& body, sf::Vector2f pos, Direction dir);

	// Reports snake's head position
	sf::Vector2f getHeadPosition() const;

	// Set snake's direction
	void setDirection(Direction dir);

	// Updates snake's position based on its move direction
	void update();

	// Display method (override sf::Drawable)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Tells the snake to grow of one snake's unit
	void grow();

	// Reports snake's size
	unsigned int size() const;

private:

	// Should the snake grow on next update call ?
	bool shouldGrow;

	// Snake's move direction
	Direction direction;

	// Sprite to use when displaying snake's head
	sf::Sprite headSprite;

	// Sprite to use when displaying snake's body part
	sf::Sprite bodySprite;
	
	// Snake's body part stored as a sprite array
	std::list<sf::Sprite> bodyPositions;

};

#endif // !SNAKE_H_INCLUDED

