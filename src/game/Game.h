#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../network/Client.h"
#include "GameState.h"

class Game {

public:

	Game(const std::string& ip, int port, const sf::Color& localPlayerColor);

	~Game();

	void MainLoop();

	void DisplayGameInConsole(GameState gameState) const;

	char drawChar(CellState state) const;

	bool LoadResources();

	bool WinCondition(const std::vector<Snake>& snakes) const;

private:

	sf::RenderWindow window;

	float GetAngleFromDirection(Direction dir) const;

	Client client;
	Direction lastDir;

	void LoadSpriteForSheet(sf::Sprite& sprite, int xOffset, int yOffset);
	
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

	sf::Text loseText;
	sf::Text winText;

	std::string ip;
	int port;

	sf::Color localPlayerColor;
	bool gameOver;
	sf::Clock gameOverClock;
};



#endif // !GAME_H_INCLUDED

