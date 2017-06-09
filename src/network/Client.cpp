#include <iostream>

#include "Network.h"
#include "Client.h"

Client::Client() : quit(false) {}

bool Client::Start(const std::string& ip, int port, const std::vector<sf::Color>& playersColor) {
	bool configureResult = Configure(ip, port, playersColor);

	if (configureResult) {
		StartThreads();
	}
	else {
		std::cout << "Configure failed !" << std::endl;
	}

	return configureResult;
}

bool Client::Configure(const std::string& ip, int port, const std::vector<sf::Color>& playersColor) {

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
	packet >> infos;
	clientInfos = infos;
	std::cout << "Received client infos !" << std::endl;


	// Receive game settings
	packet.clear();
	std::cout << "Awaiting gamesettings..." << std::endl;
	if (socket.receive(packet) != sf::Socket::Done) {
		std::cerr << "Error while receiving GameSettings..." << std::endl;
		return false;
	}
	packet >> gameSettings;
	std::cout << "Gamesettings received !" << std::endl;

	return true;
}

void Client::SetLocalPlayersInputs(const InputList& inputs) {
	
	senderMutex.lock();
	inputsToSend = inputs;
	shouldSendInputs = true;
	senderMutex.unlock();
}

bool Client::FetchInputsFromServer(InputList& inputs) {

	bool result = false;
	receiverMutex.lock();
	if (receivedServerResponse) {

		inputs = receivedInputs;
		receivedServerResponse = false;
		result = true;
	}
	receiverMutex.unlock();

	return result;
}

GameSettings Client::GetGameSettings() const
{
	return gameSettings;
}

ClientInfo Client::GetClientInfos() const {

	return clientInfos;
}

void Client::ReceiveLoop() {

	sf::Packet packet;
	while (socket.receive(packet) == sf::Socket::Done) {

		receiverMutex.lock();
		packet >> receivedInputs;
		receivedServerResponse = true;
		receiverMutex.unlock();

		packet.clear();
	}

	std::cout << "Receiver thread aborting" << std::endl;
}

void Client::SendLoop() {

	sf::Socket::Status status = sf::Socket::Done;
	sf::Packet packet;

	while (!quit) {

		senderMutex.lock();
		if (shouldSendInputs) {

			packet << inputsToSend;
			status = socket.send(packet);
			shouldSendInputs = false;
			packet.clear();
		}
		senderMutex.unlock();

		if (status != sf::Socket::Done) {
			break;
		}
	}

	std::cout << "Sender thread aborting" << std::endl;
}

void Client::StartThreads() {
	senderThread = std::thread(&Client::SendLoop, this);
	receiverThread = std::thread(&Client::ReceiveLoop, this);
}

void Client::Close() {

	quit = true;
	socket.disconnect();

	senderThread.join();
	receiverThread.join();
}