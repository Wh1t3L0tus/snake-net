#include <iostream>
#include "network/Server.h"

#include "game\Game.h"


#include <SFML/Network.hpp>


int main(int argc, char** argv)
{
	if (argc != 7) {
		std::cerr << "snake-net Usage :\n" << "snake-net.exe client ip port snake-red snake-green snake-blue\n" << "snake-net.exe server ip port nbPlayers map tickDuration" << std::endl;
		return EXIT_SUCCESS;
	}

	const std::string SERVER_MODE = "server";
	const std::string CLIENT_MODE = "client";

	if (SERVER_MODE == argv[1]) {

		std::string ip = argv[2];
		int port = std::stoi(argv[3]);
		int nbPlayers = std::stoi(argv[4]);
		std::string map = argv[5];
		float tickDuration = std::stof(argv[6]);

		Server server(ip, port, nbPlayers, tickDuration, map);
		
		std::cout << std::boolalpha << server.start() << std::endl;
	}
	else if (CLIENT_MODE == argv[1]) {

		std::string ip = argv[2];
		int port = std::stoi(argv[3]);
		int red = std::stoi(argv[4]);
		int green = std::stoi(argv[5]);
		int blue = std::stoi(argv[6]);

		Game game(ip, port, sf::Color(red, green, blue));

		if (game.LoadResources()) {
			game.MainLoop();
		}
	}
	

	return 0;
}