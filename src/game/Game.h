#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../network/Client.h"
#include "GameState.h"

enum PlayerStatus 
{
	GAME_ON,
	LOSER,
	WINNER,
	GAME_OVER
};

class Game {

public:

	Game(const std::string& ip, int port, const sf::Color& localPlayerColor);

	~Game();

	void MainLoop();

	void DisplayGameInConsole(GameState gameState) const;

	char drawChar(CellState state) const;

	bool LoadResources();

private:

	sf::RenderWindow window;
	sf::Vector2f windowSize;

	float GetAngleFromDirection(Direction dir) const;

	Client client;
	Direction lastDir;
	GameState gameState;
	GameSettings settings;

	void LoadSpriteForSheet(sf::Sprite& sprite, int xOffset, int yOffset);

	void GetAndSendPlayerInput();
	void ReceiveInputsAndUpdateGameState();
	void DrawGameState();
	void DrawCell(const CellState& cell, const sf::Vector2f& pos);
	void DrawSprite(sf::Sprite& sprite, const sf::Vector2f& position);
	void DrawSprite(sf::Sprite& sprite, const sf::Vector2f& position, const sf::Color& color);
	void DrawSprite(sf::Sprite& sprite, const sf::Vector2f& position, const sf::Color& color, const float& angle);

	int GetAliveSnakesCount() const;
	std::vector<sf::Vector2f> explosionsPositions;
	
	sf::Texture titleTexture;
	sf::Texture spriteSheet;
	sf::Texture explosionTexture;
	
	sf::Sprite titleSprite;
	sf::Sprite appleSprite;
	sf::Sprite backgroundSprite;
	sf::Sprite wallSprite;
	sf::Sprite snakeHeadSprite;
	sf::Sprite snakeBodySprite;
	sf::Sprite explosionSprite;

	sf::Font font;
	
	sf::Text waitingText;
	sf::Text loseText;
	sf::Text winText;

	std::string ip;
	int port;

	PlayerStatus playerStatus;

	sf::Color localPlayerColor;
	bool gameOver;
	sf::Clock gameOverClock;
};



#endif // !GAME_H_INCLUDED

