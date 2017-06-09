#include <SFML/Graphics.hpp>
#include <iostream>


#include "Game.h"
#include "Direction.h"

float TILE_SIZE = 20.0f;

Game::Game(const std::string& ip, int port, const sf::Color& localPlayerColor) :
	ip(ip),
	port(port),
	localPlayerColor(localPlayerColor) 
{
	gamePhase = CREATE_WINDOW;
}

Game::~Game() {
	client.Close();
}

bool Game::LoadResources() {

	if (!spriteSheet.loadFromFile("Assets/spritesheet.png") ||
		!explosionTexture.loadFromFile("Assets/explosion.png") ||
		!titleTexture.loadFromFile("Assets/title.png"))
	{
		return false;
	}

	titleSprite.setTexture(titleTexture);
	titleSprite.setOrigin(titleTexture.getSize().x / 2, titleTexture.getSize().y / 2);

	LoadSpriteForSheet(wallSprite, 5, 0);
	LoadSpriteForSheet(backgroundSprite, 5, 1);
	LoadSpriteForSheet(snakeHeadSprite, 0, 2);
	LoadSpriteForSheet(snakeBodySprite, 0, 3);
	LoadSpriteForSheet(appleSprite, 0, 4);
	
	explosionSprite.setTexture(explosionTexture);
	explosionSprite.setOrigin(TILE_SIZE * 1.5f / 2.0f, TILE_SIZE * 1.5f / 2.0f);

	return true;
}

void Game::MainLoop() {

	std::vector<sf::Color> localPlayersColors;
	sf::Vector2f windowSize(800, 600);

	GameState gameState;
	GameSettings settings;

	sf::VideoMode VMode(windowSize.x, windowSize.y);
	window.create(VMode, "Snake-net-client");

	sf::Vector2f titleSize(titleSprite.getGlobalBounds().width * 1.25f, titleSprite.getGlobalBounds().height * 1.25f);

	if (titleSize.x > windowSize.x)
	{
		titleSprite.scale(windowSize.x / titleSize.x, windowSize.x / titleSize.x);
	}
	if (titleSize.y > windowSize.y)
	{
		titleSprite.scale(windowSize.y / titleSize.y, windowSize.y / titleSize.y);
	}

	sf::Vector2f titlePos = windowSize;
	titlePos.x /= 2;
	titlePos.y /= 4;

	titleSprite.setPosition(titlePos);

	window.clear();
	window.draw(titleSprite);
	window.display();


	localPlayersColors.push_back(localPlayerColor);

	if (!client.Start(ip, port, localPlayersColors)) {
		std::cerr << "Error while starting client" << std::endl;
		return;
	}

	settings = client.GetGameSettings();

	if (!gameState.Initialize(settings)) {
		return;
	}


	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				window.close();
				return;
			}
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

			//DisplayGameInConsole(gameState);
		}

		// Draw window
		window.clear();

		const std::vector<Snake>& snakes = gameState.GetSnakes();
		std::vector<CellState> map = gameState.GetMap();
		for (int j = 0; j < gameState.height; j++) {

			for (int i = 0; i < gameState.width; i++) {

				CellState cell = map[j * gameState.width + i];
				sf::Vector2f position = sf::Vector2f(TILE_SIZE * i, TILE_SIZE * j) + windowSize / 2.0f - sf::Vector2f(gameState.width * TILE_SIZE / 2.0f, gameState.height * TILE_SIZE / 2.0f);
				sf::Vector2f centeredPosition = position + sf::Vector2f(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f);

				backgroundSprite.setPosition(centeredPosition);
				wallSprite.setPosition(centeredPosition);
				snakeBodySprite.setPosition(centeredPosition);
				snakeHeadSprite.setPosition(centeredPosition);
				appleSprite.setPosition(centeredPosition);
				explosionSprite.setPosition(centeredPosition);

				window.draw(backgroundSprite);

				switch (cell)
				{
					case EMPTY:
						window.draw(backgroundSprite);
						break;
					case WALL:
						window.draw(wallSprite);
						break;
					case APPLE:
						window.draw(appleSprite);
						break;
					case SNAKE_0:
						snakeBodySprite.setColor(settings.playerColors[0]);
						window.draw(snakeBodySprite);
						break;
					case SNAKE_0_H:
						snakeHeadSprite.setRotation(GetAngleFromDirection(snakes[0].GetDirection()));
						snakeHeadSprite.setColor(settings.playerColors[0]);
						window.draw(snakeHeadSprite);
						break;
					case SNAKE_1:
						snakeBodySprite.setColor(settings.playerColors[1]);
						window.draw(snakeBodySprite);
						break;
					case SNAKE_1_H:
						snakeHeadSprite.setRotation(GetAngleFromDirection(snakes[1].GetDirection()));
						snakeHeadSprite.setColor(settings.playerColors[1]);
						window.draw(snakeHeadSprite);
						break;
					case SNAKE_2:
						snakeBodySprite.setColor(settings.playerColors[2]);
						window.draw(snakeBodySprite);
						break;
					case SNAKE_2_H:
						snakeHeadSprite.setRotation(GetAngleFromDirection(snakes[2].GetDirection()));
						snakeHeadSprite.setColor(settings.playerColors[2]);
						window.draw(snakeHeadSprite);
						break;
					case SNAKE_3:
						snakeBodySprite.setColor(settings.playerColors[3]);
						window.draw(snakeBodySprite);
						break;
					case SNAKE_3_H:
						snakeHeadSprite.setRotation(GetAngleFromDirection(snakes[3].GetDirection()));
						snakeHeadSprite.setColor(settings.playerColors[3]);
						window.draw(snakeHeadSprite);
						break;
					case SNAKE_4:
						snakeBodySprite.setColor(settings.playerColors[4]);
						window.draw(snakeBodySprite);
						break;
					case SNAKE_4_H:
						snakeHeadSprite.setRotation(GetAngleFromDirection(snakes[4].GetDirection()));
						snakeHeadSprite.setColor(settings.playerColors[4]);
						window.draw(snakeHeadSprite);
						break;
					default:
						window.draw(explosionSprite);
						break;
				}
			}
		}

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

void Game::LoadSpriteForSheet(sf::Sprite& sprite, int xOffset, int yOffset)
{
	sprite.setTexture(spriteSheet);
	sprite.setTextureRect(sf::IntRect(xOffset * TILE_SIZE, yOffset * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	sprite.setOrigin(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f);
}

float Game::GetAngleFromDirection(Direction dir) const {

	switch (dir)
	{
		case UP:
			return 0.0f;
		case DOWN:
			return 180.0f;
		case RIGHT:
			return 90.0f;
		case LEFT:
			return 270.0f;
		default:
			return 0.0f;
	}
}