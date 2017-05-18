#include <iostream>

#include "Network.h"
#include "Client.h"

Client::Client() {}

bool Client::Start(const std::string& ip, int port, const std::vector<sf::Color>& playersColor) {

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(ip, port);
	if (status != sf::Socket::Done)
	{
		std::cerr << "Couldn't connect to server " << ip << ":" << port << std::endl;
		return false;
	}
	std::cout << "Connected to server !" << std::endl;

	// Prepare a message with client informations
	ClientInfo infos;
	infos.localPlayerCount = playersColor.size();
	for (int i = 0; i < infos.localPlayerCount; i++) {

		infos.infos.push_back(std::pair<int, sf::Color>(-1, playersColor[0]));
	}

	// Send client infos
	sf::Packet packet;
	packet << infos;
	if (socket.send(packet) != sf::Socket::Done) {

		std::cerr << "Error : couldn't send client infos to server..." << std::endl;
		return false;
	}
	std::cout << "Client infos sent to server !" << std::endl;

	// Receive client info updated with player's IDs
	packet.clear();
	if (socket.receive(packet) != sf::Socket::Done) {

		std::cerr << "Error while receiving player's IDs..." << std::endl;
		return false;
	}
	std::cout << "Received client infos !" << std::endl;
	packet >> infos;


	// Receive game settings
	GameSettings gameSettings;
	packet.clear();
	std::cout << "Awaiting gamesettings..." << std::endl;
	if (socket.receive(packet) != sf::Socket::Done) {
		std::cerr << "Error while receiving GameSettings..." << std::endl;
		return false;
	}
	std::cout << "Gamesettings received !" << std::endl;

	return true;
}

