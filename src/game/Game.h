#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../network/Client.h"

class Game {

public:

	Game();

	void MainLoop();

private:

	Client client;
	Direction lastDir;

};



#endif // !GAME_H_INCLUDED

