#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../network/Client.h"
#include "GameState.h"

class Game {

public:

	Game();

	void MainLoop();

	void DisplayGameInConsole(GameState gameState) const;

	char drawChar(CellState state) const;

private:

	float GetAngleFromDirection(Direction dir) const;

	Client client;
	Direction lastDir;

	sf::Color getRandomColor() const;

	sf::Sprite wallSprite;
	sf::Sprite snakeHeadSprite;
	sf::Sprite snakeBodySprite;
	
	sf::Texture wallTexture;
	sf::Texture snakeTexture;
};



#endif // !GAME_H_INCLUDED

