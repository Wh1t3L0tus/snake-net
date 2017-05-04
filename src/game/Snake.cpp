#include "Snake.h"

#include <iostream>

Snake::Snake(sf::Sprite& head, sf::Sprite& body, sf::Vector2f pos, Direction dir) {

	headSprite = head;
	bodySprite = body;

	sf::Vector2f offset(headSprite.getTextureRect().width / 2.0f, headSprite.getTextureRect().height / 2.0f);

	headSprite.setOrigin(offset);
	bodySprite.setOrigin(offset);

	direction = dir;

	headSprite.setPosition(pos);
	bodyPositions.push_back(headSprite);
}

sf::Vector2f Snake::getHeadPosition() const {
	
	return bodyPositions.front().getPosition();
}

void Snake::setDirection(Direction dir) {
	
	if (dir != Direction::NONE) {

		direction = dir;
	}
}

void Snake::update() {

	float w = static_cast<float>(headSprite.getTextureRect().width);
	float h = static_cast<float>(headSprite.getTextureRect().height);

	sf::Sprite& head = bodyPositions.front();
	sf::Vector2f move(0.0f, 0.0f);

	// Compute movement based on snake's direction
	// and turn head sprite accordingly
	if (direction == Direction::UP) {

		move.y = -h;
		head.setRotation(0.0f);
	}
	else if (direction == Direction::DOWN) {

		move.y = h;
		head.setRotation(180.0f);
	}
	else if (direction == Direction::LEFT) {

		move.x = -w;
		head.setRotation(-90.0f);
	}
	else if (direction == Direction::RIGHT) {

		move.x = w;
		head.setRotation(90.0f);
	}

	// Update all body part positions
	sf::Vector2f lastPos = bodyPositions.front().getPosition() + move;
	for (auto it = bodyPositions.begin(); it != bodyPositions.end(); ++it) {

		sf::Vector2f currentPos = (*it).getPosition();
		(*it).setPosition(lastPos);
		lastPos = currentPos;
	}

	// Grow our snake if it needs to grow
	if (shouldGrow) {

		bodySprite.setPosition(lastPos);
		bodyPositions.push_back(bodySprite);
		shouldGrow = false;
	}
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	sf::Sprite head = headSprite;
	sf::Sprite body = bodySprite;
	
	for (auto it = bodyPositions.begin(); it != bodyPositions.end(); ++it) {

		target.draw(*it, states);
	}
}

void Snake::grow() {

	shouldGrow = true;
}

unsigned int Snake::size() const {

	return bodyPositions.size();
}
