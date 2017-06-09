#include <iostream>
#include "network/Server.h"

#include "game\Game.h"


#include <SFML/Network.hpp>


int main(int argc, char** argv)
{
	if (argc != 7 && argc != 2) {
		std::cerr << "snake-net Usage :\n" << "snake-net.exe client ip port snake-red snake-green snake-blue\n" << "snake-net.exe server ip port nbPlayers map tickDuration" << std::endl;
		return EXIT_SUCCESS;
	}

	const std::string SERVER_MODE = "server";
	const std::string CLIENT_MODE = "client";

	bool simplifiedMode = argc == 2;

	if (SERVER_MODE == argv[1]) {

		std::string ip;
		int port;
		int nbPlayers;
		std::string map;
		float tickDuration;

		if (simplifiedMode) {
			
			std::cout << "Host IP : ";
			std::cin >> ip;
			std::cout << std::endl << "Listen port : ";
			std::cin >> port;
			std::cout << std::endl << "Number of players : ";
			std::cin >> nbPlayers;
			std::cout << std::endl << "Map name : ";
			std::cin >> map;
			std::cout << std::endl << "Tick duration : ";
			std::cin >> tickDuration;
		}
		else {
			ip = argv[2];
			port = std::stoi(argv[3]);
			nbPlayers = std::stoi(argv[4]);
			map = argv[5];
			tickDuration = std::stof(argv[6]);
		}


		Server server(ip, port, nbPlayers, tickDuration, map);
		
		std::cout << std::boolalpha << server.start() << std::endl;
	}
	else if (CLIENT_MODE == argv[1]) {

		std::string ip;
		int port;
		int red;
		int green;
		int blue;

		if (simplifiedMode) {
			std::cout << "Server IP : ";
			std::cin >> ip;
			std::cout << std::endl << "Server port : ";
			std::cin >> port;
			std::cout << std::endl << "Snake red component : ";
			std::cin >> red;
			std::cout << std::endl << "Snake green component : ";
			std::cin >> green;
			std::cout << std::endl << "Snake blue component : ";
			std::cin >> blue;
		}
		else {
			ip = argv[2];
			port = std::stoi(argv[3]);
			red = std::stoi(argv[4]);
			green = std::stoi(argv[5]);
			blue = std::stoi(argv[6]);
		}


		Game game(ip, port, sf::Color(red, green, blue));

		if (game.LoadResources()) {
			game.MainLoop();
		}
	}
	

	return 0;
}