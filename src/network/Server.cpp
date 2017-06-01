#include <iostream>
#include "Network.h"
#include "Server.h"


Server::Server(const std::string& ip, int port, int playerCount, float tickDuration, std::string mapName) :
	ip(ip), 
	port(port), 
	playerCount(playerCount),
	tickDuration(tickDuration),
	mapName(mapName) {}

Server::~Server() {

	for (auto it = clients.begin(); it != clients.end(); ++it) {

		std::cout << "Waiting completion of client thread for socket " << it->socket->getRemoteAddress() << ":" << it->socket->getRemotePort() << std::endl;
		it->socket->disconnect();
		it->listenerThread.join();
	}
}

bool Server::start() {

	// Bind server socket to port
	if (listener.listen(port) != sf::Socket::Done) {

		std::cerr << "Error while binding socket to port " << port << std::endl;
		return false;
	}

	if (!lobbyLoop()) {
		std::cerr << "Error while accepting connections" << std::endl;
		return false;
	}

	// At this point, we have all our clients
	// We can send GameSettings to every client
	GameSettings settings = createGameSettings();
	sf::Packet packet;
	packet << settings;
	for (int i = 0; i < clients.size(); i++) {

		if (clients[i].socket->send(packet) != sf::Socket::Done) {
			std::cerr << "Error while sending GameSettings to client " << i << std::endl;
			return false;
		}
	}

	startListenerThreads();

	gameLoop();

	return true;
}

bool Server::lobbyLoop() {

	int connectedPlayers = 0;
	while (connectedPlayers != playerCount) {

		// Accept incoming client
		std::shared_ptr<sf::TcpSocket> client = std::shared_ptr<sf::TcpSocket>(new sf::TcpSocket());
		if (listener.accept(*client) != sf::Socket::Done) {

			std::cerr << "Warning : lobbyLoop() accept failed" << std::endl;
			continue;
		}

		// onAccept -> receive client info, send back client id
		if (!onAcceptClient(client, connectedPlayers)) {

			continue;
		}
	}

	return true;
}

void Server::gameLoop() {

	
	sf::Time elapsedTime = sf::seconds(0);
	sf::Clock clock;

	InputList inputList{
		playerCount,
		std::vector<Direction>(playerCount)
	};

	bool quit = false;
	while (!quit) {

		if ((clock.getElapsedTime() - elapsedTime).asSeconds() >= tickDuration) {

			fillInputList(inputList);

			if (!broadcastInputList(inputList)) {
				std::cerr << "Error couldn't send input list to all clients" << std::endl;
				quit = true;
			}

			elapsedTime = clock.getElapsedTime();
		}
	}
	std::cout << "Leaving game loop" << std::endl;
}

bool Server::onAcceptClient(std::shared_ptr<sf::TcpSocket> clientSocket, int& connectedPlayers) {

	sf::Packet packet;
	if (clientSocket->receive(packet) != sf::Socket::Done) {

		std::cerr << "Warning : onAcceptClient() couldn't receive client info" << std::endl;
		return false;
	}

	ClientInfo info;
	packet >> info;
	packet.clear();

	std::vector<Player> playerList;
	for (char i = 0; i < info.localPlayerCount; i++) {

		playerList.push_back(
			Player {
				(char)connectedPlayers,
				info.infos[i].second,
				Direction::NONE
			}
		);

		info.infos[i].first = connectedPlayers;
		connectedPlayers++;
	}

	clients.push_back(
		ClientStruct {
			clientSocket,
			playerList
		}
	);

	packet << info;
	if (clientSocket->send(packet) != sf::Socket::Done) {

		std::cerr << "Error : onAcceptClient() couldn't send back client info" << std::endl;
		
		// Rollback : erase last inserted client
		clients.erase(clients.end() - 1);
		connectedPlayers -= info.localPlayerCount;
	}

	return true;
}

GameSettings Server::createGameSettings() {

	srand(time(0));
	GameSettings s;
	
	s.seed = rand();
	s.playerCount = playerCount;
	s.mapName = mapName;
	s.tickDuration = tickDuration;

	for (int i = 0; i < clients.size(); i++) {

		for (int j = 0; j < clients[i].players.size(); j++) {

			s.playerColors.push_back(clients[i].players[j].color);
		}
	}

	return s;
}

void Server::startListenerThreads() {

	for (auto it = clients.begin(); it != clients.end(); ++it) {

		std::shared_ptr<sf::TcpSocket> socket = it->socket;
		std::vector<Player>& players = it->players;

		it->listenerThread = std::thread([socket, players] () mutable {

			sf::Packet packet;
			InputList inputList;

			std::cout << "Started thread for " << socket->getRemoteAddress() << ":" << socket->getRemotePort() << std::endl;
			while (socket->receive(packet) == sf::Socket::Done) {

				packet >> inputList;
				packet.clear();

				for (int i = 0; i < inputList.nbInput; i++) {
					
					players[i].direction = inputList.inputs[i];
				}
				std::cout << "Received input for " << socket->getRemoteAddress() << ":" << socket->getRemotePort() << std::endl;
			}  

			std::cout << "Thread terminated " << socket->getRemoteAddress() << " " << socket->getRemotePort() << std::endl;
		});
	}
}

void Server::fillInputList(InputList& inputList) {

	int count = 0;
	for (int i = 0; i < clients.size(); i++) {

		for (int j = 0; j < clients[i].players.size(); j++) {

			inputList.inputs[count] = clients[i].players[j].direction;
			count++;
		}
	}
}

bool Server::broadcastInputList(const InputList& inputList) {

	sf::Packet packet;
	packet << inputList;

	for (int i = 0; i < clients.size(); i++) {

		if (clients[i].socket->send(packet) != sf::Socket::Done) {

			return false;
		}
	}

	return true;
}