#include <SFML/Graphics.hpp>
#include <iostream>


#include "Game.h"
#include "Direction.h"

Game::Game() {

}

void Game::MainLoop() {

	std::vector<sf::Color> playerColors;
	playerColors.push_back(getRandomColor());

	if (!client.Start("127.0.0.1", 8888, playerColors)) {
		std::cerr << "Error while starting client" << std::endl;
		return;
	}

	GameState gameState;
	gameState.Initialize(client.GetGameSettings());
	std::cout << std::endl;
	std::vector<CellState> mapState = gameState.GetMap();
	for (size_t j = 0; j < gameState.height; j++)
	{
		for (size_t i = 0; i < gameState.width; i++)
		{
			std::cout << drawChar(mapState[i + j * gameState.width]);
		}
		std::cout << std::endl;
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Snake-net-client");

	while (window.isOpen()) {

		//std::cerr << "frame" << std::endl;

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

		if (dir != Direction::NONE && dir != lastDir) {
			InputList localInputs;
			localInputs.nbInput = 1;
			localInputs.inputs.push_back(dir);
			client.SetLocalPlayersInputs(localInputs);
		}

		lastDir = dir;

		InputList fetchedInputs;
		if (client.FetchInputsFromServer(fetchedInputs)) {
			// update game state
			gameState.Update(fetchedInputs);

			//DisplayGameInConsole();
		}

		// Draw window
		window.clear();


		window.display();
	}
}

void Game::DisplayGameInConsole(GameState gameState) const 
{
	std::cout << std::endl;
	std::vector<CellState> mapState = gameState.GetMap();
	for (size_t j = 0; j < gameState.height; j++)
	{
		for (size_t i = 0; i < gameState.width; i++)
		{
			std::cout << drawChar(mapState[i + j * gameState.width]);
		}
		std::cout << std::endl;
	}
}

char Game::drawChar(CellState state) const
{
	switch (state)
	{
		case EMPTY: return ' ';
		case WALL: return '#';
		case APPLE: return '0';
		case SNAKE_0: return 'a';
		case SNAKE_0_H: return 'A';
		case SNAKE_1: return 'b';
		case SNAKE_1_H: return 'B';
		case SNAKE_2: return 'c';
		case SNAKE_2_H: return 'C';
		case SNAKE_3: return 'd';
		case SNAKE_3_H: return 'D';
		case SNAKE_4: return 'e'; 
		case SNAKE_4_H: return 'E';
		default: return 'X';
	}
}

sf::Color Game::getRandomColor() const
{
	int r = (int)(((double)rand() / RAND_MAX) * 8);
	switch (r)
	{
		case 0: return sf::Color::Black;
		case 1: return sf::Color::Blue;
		case 2: return sf::Color::Cyan;
		case 3: return sf::Color::Green;
		case 4: return sf::Color::Magenta;
		case 5: return sf::Color::Red;
		case 6: return sf::Color::Yellow;
		default: return sf::Color::White;
	}
}
