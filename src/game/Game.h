#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../network/Client.h"
#include "GameState.h"

class Game {

public:

	Game(const std::string& ip, int port, const sf::Color& localPlayerColor);

	void MainLoop();

	void DisplayGameInConsole(GameState gameState) const;

	char drawChar(CellState state) const;

	bool LoadResources();

private:

	float GetAngleFromDirection(Direction dir) const;

	Client client;
	Direction lastDir;

	sf::Color getRandomColor() const;

	sf::Sprite appleSprite;
	sf::Sprite explosionSprite;
	sf::Sprite backgroundSprite;
	sf::Sprite wallSprite;
	sf::Sprite snakeHeadSprite;
	sf::Sprite snakeBodySprite;
	
	sf::Texture appleTexture;
	sf::Texture explosionTexture;
	sf::Texture mapTexture;
	sf::Texture snakeTexture;

	std::string ip;
	int port;

	sf::Color localPlayerColor;
};



#endif // !GAME_H_INCLUDED

