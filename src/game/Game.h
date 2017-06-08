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

	Client client;
	Direction lastDir;

	sf::Color getRandomColor() const;
};



#endif // !GAME_H_INCLUDED

