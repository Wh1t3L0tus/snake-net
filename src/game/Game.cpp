#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"
#include "Direction.h"

float TILE_SIZE = 20.0f;

const bool USE_GAME_OVER_CLOCK = true;

const std::string PATH_IMAGES = "Assets/images/";
const std::string PATH_FONTS = "Assets/fonts/";
const std::string PATH_MAPS = "Assets/maps/";

Game::Game(const std::string& ip, int port, const sf::Color& localPlayerColor) :
	ip(ip),
	port(port),
	localPlayerColor(localPlayerColor),
	gameOver(false)
{
}


Game::~Game() {
	client.Close();
}

bool Game::LoadResources() {

	if (!spriteSheet.loadFromFile(PATH_IMAGES + "spritesheet.png") ||
		!explosionTexture.loadFromFile(PATH_IMAGES + "explosion.png") ||
		!font.loadFromFile(PATH_FONTS + "Roboto/Roboto-Regular.ttf") ||
		!titleTexture.loadFromFile(PATH_IMAGES + "title.png"))
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

	waitingText.setFont(font);
	loseText.setFont(font);
	winText.setFont(font);

	waitingText.setCharacterSize(40);
	loseText.setCharacterSize(50);
	winText.setCharacterSize(50);
	
	waitingText.setFillColor(sf::Color::Red);
	loseText.setFillColor(sf::Color::Red);
	winText.setFillColor(sf::Color::Red);

	loseText.setString("You lose !");
	winText.setString("You win !");


	return true;
}

void Game::MainLoop() {

	std::vector<sf::Color> localPlayersColors;
	windowSize = sf::Vector2f(800, 600);

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
	waitingText.setString("Connection with server established.");
	waitingText.setPosition(sf::Vector2f(windowSize.x / 2.0f - waitingText.getGlobalBounds().width / 2, windowSize.y / 2.0f));
	window.draw(waitingText);
	waitingText.setString("Waiting for other players.");
	waitingText.setPosition(sf::Vector2f(windowSize.x / 2.0f - waitingText.getGlobalBounds().width / 2, windowSize.y / 2.0f + 2 * waitingText.getGlobalBounds().height));
	window.draw(waitingText);

	window.display();


	localPlayersColors.push_back(localPlayerColor);

	if (!client.Start(ip, port, localPlayersColors)) {
		std::cerr << "Error while starting client" << std::endl;
		return;
	}

	settings = client.GetGameSettings();
	int localSnakeID = client.GetClientInfos().infos[0].first;

	if (!gameState.Initialize(settings)) {
		return;
	}

	loseText.setPosition(windowSize / 2.0f - sf::Vector2f(loseText.getGlobalBounds().width / 2.0f, loseText.getGlobalBounds().height / 2.0f));
	winText.setPosition(windowSize / 2.0f - sf::Vector2f(winText.getGlobalBounds().width / 2.0f, winText.getGlobalBounds().height / 2.0f));

	playerStatus = GAME_ON;

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

		const std::vector<Snake>& snakes = gameState.GetSnakes();

		switch (playerStatus)
		{
			case GAME_ON:
				GetAndSendPlayerInput();
				ReceiveInputsAndUpdateGameState();

				window.clear();
				DrawGameState();

				{
					int snakeCount = GetAliveSnakesCount();
					if (snakeCount == 0)
					{
						bool amILongest = true;
						for (int i = 0; i < snakes.size(); i++)
						{
							if (snakes[i].bodyPositions.size() > snakes[localSnakeID].bodyPositions.size()) {
								amILongest = false;
								break;
							}
						}
						window.draw(amILongest ? winText : loseText);
						playerStatus = GAME_OVER;
						std::cout << "GAME_OVER" << std::endl;

					}
					else if (snakes[localSnakeID].isDead)
					{
						playerStatus = LOSER;
						std::cout << "LOSER" << std::endl;
					}
					else if (snakeCount == 1)
					{
						playerStatus = WINNER;
						std::cout << "WINNER" << std::endl;
					}
				}

				window.display();

				break;

			case LOSER:
				ReceiveInputsAndUpdateGameState();

				window.clear();
				DrawGameState();
				window.draw(loseText);
				window.display();

				if (GetAliveSnakesCount() == 0)
				{
					playerStatus = GAME_OVER;
					std::cout << "GAME_OVER" << std::endl;
				}

				break;

			case WINNER:
				GetAndSendPlayerInput();
				ReceiveInputsAndUpdateGameState();

				window.clear();
				DrawGameState();
				window.draw(winText);
				window.display();

				if (GetAliveSnakesCount() == 0)
				{
					playerStatus = GAME_OVER;
					std::cout << "GAME_OVER" << std::endl;
				}

				break;

			case GAME_OVER:
				if (USE_GAME_OVER_CLOCK && !gameOver)
				{
					gameOverClock.restart();
					gameOver = true;
				}
				else if (gameOverClock.getElapsedTime().asSeconds() > 5.0f)
				{
					window.close();
				}
				break;
		}
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

void Game::GetAndSendPlayerInput()
{
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
}

void Game::ReceiveInputsAndUpdateGameState()
{
	InputList fetchedInputs;
	if (client.FetchInputsFromServer(fetchedInputs)) 
	{
		gameState.Update(fetchedInputs);
	}
}

void Game::DrawGameState()
{
	std::vector<CellState> map = gameState.GetMap();

	explosionsPositions.clear();

	for (int j = 0; j < gameState.height; j++) {

		for (int i = 0; i < gameState.width; i++) {

			CellState cell = map[j * gameState.width + i];
			sf::Vector2f position = sf::Vector2f(TILE_SIZE * i, TILE_SIZE * j) + windowSize / 2.0f - sf::Vector2f(gameState.width * TILE_SIZE / 2.0f, gameState.height * TILE_SIZE / 2.0f);
			sf::Vector2f centeredPosition = position + sf::Vector2f(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f);

			DrawSprite(backgroundSprite, centeredPosition);

			DrawCell(cell, centeredPosition);
		}
	}

	for (int i = 0; i < explosionsPositions.size(); i++)
	{
		explosionSprite.setPosition(explosionsPositions[i]);
		window.draw(explosionSprite);
	}
}

void Game::DrawCell(const CellState& cell, const sf::Vector2f& pos)
{
	const std::vector<Snake>& snakes = gameState.GetSnakes();

	int count = 0;

	if (cell == EMPTY)
	{
		return;
	}
	else if (cell == APPLE)
	{
		DrawSprite(appleSprite, pos);
		return;
	}
	else if (cell == (CellState)(cell | WALL))
	{
		DrawSprite(wallSprite, pos);
		count++;
	}
	else if (cell == (CellState)(cell | SNAKE_0))
	{
		DrawSprite(snakeBodySprite, pos, settings.playerColors[0]);
		count++;
	}
	else if (cell == (CellState)(cell | SNAKE_1))
	{
		DrawSprite(snakeBodySprite, pos, settings.playerColors[1]);
		count++;
	}
	else if (cell == (CellState)(cell | SNAKE_2))
	{
		DrawSprite(snakeBodySprite, pos, settings.playerColors[2]);
		count++;
	}
	else if (cell == (CellState)(cell | SNAKE_3))
	{
		DrawSprite(snakeBodySprite, pos, settings.playerColors[3]);
		count++;
	}
	else if (cell == (CellState)(cell | SNAKE_4))
	{
		DrawSprite(snakeBodySprite, pos, settings.playerColors[4]);
		count++;
	}

	if (cell == (CellState)(cell | SNAKE_0_H))
	{
		DrawSprite(snakeHeadSprite, pos, settings.playerColors[0], GetAngleFromDirection(snakes[0].GetDirection()));
		count++;
	}
	if (cell == (CellState)(cell | SNAKE_1_H))
	{
		DrawSprite(snakeHeadSprite, pos, settings.playerColors[1], GetAngleFromDirection(snakes[1].GetDirection()));
		count++;
	}
	if (cell == (CellState)(cell | SNAKE_2_H))
	{
		DrawSprite(snakeHeadSprite, pos, settings.playerColors[2], GetAngleFromDirection(snakes[2].GetDirection()));
		count++;
	}
	if (cell == (CellState)(cell | SNAKE_3_H))
	{
		DrawSprite(snakeHeadSprite, pos, settings.playerColors[3], GetAngleFromDirection(snakes[3].GetDirection()));
		count++;
	}
	if (cell == (CellState)(cell | SNAKE_4_H))
	{
		DrawSprite(snakeHeadSprite, pos, settings.playerColors[4], GetAngleFromDirection(snakes[4].GetDirection()));
		count++;
	}

	if (count > 1)
	{
		explosionsPositions.push_back(pos);
	}
}

void Game::DrawSprite(sf::Sprite& sprite, const sf::Vector2f& position)
{
	sprite.setPosition(position);
	window.draw(sprite);
}

void Game::DrawSprite(sf::Sprite & sprite, const sf::Vector2f & position, const sf::Color & color)
{
	sprite.setColor(color);
	DrawSprite(sprite, position);
}

void Game::DrawSprite(sf::Sprite & sprite, const sf::Vector2f & position, const sf::Color & color, const float & angle)
{
	sprite.setRotation(angle);
	DrawSprite(sprite, position, color);
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

int Game::GetAliveSnakesCount() const
{
	const std::vector<Snake>& snakes = gameState.GetSnakes();

	int aliveSnakes = 0;

	for (int i = 0; i < snakes.size(); i++) {

		if (!snakes[i].isDead) {
			aliveSnakes++;
		}
	}

	return aliveSnakes;
}
