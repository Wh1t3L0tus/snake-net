#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime> // time(0) for srand()

#include "game/Snake.h"

const std::string SNAKE_DEFAULT_SPRITE = "Assets/snake-default.png";

float Random01() 
{
	return ((float)std::rand() / (RAND_MAX)) + 1.0f;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);

	sf::Texture headTex;
	sf::Texture bodyTex;

	if (!headTex.loadFromFile(SNAKE_DEFAULT_SPRITE, sf::IntRect(0, 0, 20, 20))) {
		std::cerr << "Couldn't load head from snake-default.png" << std::endl;
		return EXIT_FAILURE;
	}
	if (!bodyTex.loadFromFile(SNAKE_DEFAULT_SPRITE, sf::IntRect(20, 0, 20, 20))) {
		std::cerr << "Couldn't load body from snake-default.png" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Sprite head(headTex); 
	sf::Sprite body(bodyTex);

	std::srand(time(0));
	sf::Color clr = sf::Color(Random01() * 255, Random01() * 255, Random01() * 255);
	std::cout << "R : " << (int)clr.r << " G : " << (int)clr.g << " B : " << (int)clr.b << std::endl;
	body.setColor(clr);
	head.setColor(clr);

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