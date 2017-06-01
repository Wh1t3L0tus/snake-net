#include <SFML/Graphics.hpp>
#include <iostream>


#include "Game.h"
#include "../game/Direction.h"

Game::Game() {

}

void Game::MainLoop() {

	std::vector<sf::Color> playerColors;
	playerColors.push_back(sf::Color::Red);

	if (!client.Start("127.0.0.1", 8888, playerColors)) {
		std::cerr << "Error while starting client" << std::endl;
		return;
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Snake-net-client");

	while (window.isOpen()) {

		std::cerr << "frame" << std::endl;

		// Read mouse and keyboard inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		Direction dir = Direction::NONE;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

			dir = Direction::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

			dir = Direction::RIGHT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {

			dir = Direction::UP;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

			dir = Direction::DOWN;
		}

		if (dir != Direction::NONE) {
			InputList localInputs;
			localInputs.nbInput = 1;
			localInputs.inputs.push_back(dir);
			client.SetLocalPlayersInputs(localInputs);
		}

		InputList fetchedInputs;
		if (client.FetchInputsFromServer(fetchedInputs)) {
			// update game state
			for (int i = 0; i < fetchedInputs.inputs.size(); i++) {
				std::cout << "Player " << i << " input " << fetchedInputs.inputs[i] << std::endl;
			}
		}

		// Draw window
		window.clear();

		window.display();
	}
}