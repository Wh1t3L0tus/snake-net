#include <iostream>
#include <SFML/Graphics.hpp>

#include "game/Snake.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);

	sf::Texture headTex;
	sf::Texture bodyTex;
	if (!headTex.loadFromFile("Assets/snake-head2.png")) {
		std::cerr << "Couldn't load snake-head2.png" << std::endl;
		return EXIT_FAILURE;
	}

	if (!bodyTex.loadFromFile("Assets/snake-body2.png")) {
		std::cerr << "Couldn't load snake-body2.png" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Sprite head(headTex); 
	sf::Sprite body(bodyTex);

	Snake snake(head, body, sf::Vector2f(400.0f, 300.0f), Snake::Direction::RIGHT);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		Snake::Direction dir = Snake::Direction::NONE;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			
			dir = Snake::Direction::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

			dir = Snake::Direction::RIGHT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {

			dir = Snake::Direction::UP;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

			dir = Snake::Direction::DOWN;
		}

		snake.setDirection(dir);
		snake.grow();

		snake.update();

		window.clear();
		window.draw(snake);
		window.display();

		sf::sleep(sf::seconds(0.1f));
	}

	return 0;
}