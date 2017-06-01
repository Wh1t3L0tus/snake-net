#include <iostream>
#include "network/Server.h"

#include "game\Game.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Launch mode required : server or client ?" << std::endl;
	}

	const std::string SERVER_MODE = "server";
	const std::string CLIENT_MODE = "client";

	if (SERVER_MODE == argv[1]) {

		Server server("127.0.0.1", 8888, 2, 0.5f, "test");
		
		std::cout << std::boolalpha << server.start() << std::endl;
	}
	else if (CLIENT_MODE == argv[1]) {

		Game game;
		game.MainLoop();
	}

	return 0;
}