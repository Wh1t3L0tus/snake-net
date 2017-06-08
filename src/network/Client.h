#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Network.hpp>
#include <SFML/Graphics/Color.hpp>

#include <thread>
#include <mutex>

#include "../network/Network.h"

class Client {

public:

	Client();

	bool Start(const std::string& ip, int port, const std::vector<sf::Color>& playersColor);

	// Set inputs of every local players to send to the server
	void SetLocalPlayersInputs(const InputList& inputs);
	
	// Returns true if inputs were received from server and fill "inputs"
	bool FetchInputsFromServer(InputList& inputs);

	GameSettings GetGameSettings() const;

private:

	bool Configure(const std::string& ip, int port, const std::vector<sf::Color>& playersColor);
	void StartThreads();

	// Treaded function for receiving inputs from server.
	void ReceiveLoop();
	// Threaded function for sending inputs to server.
	void SendLoop();

	InputList inputsToSend;
	InputList receivedInputs;
	bool receivedServerResponse;
	bool shouldSendInputs;
	std::thread receiverThread;
	std::thread senderThread;
	sf::TcpSocket socket;

	GameSettings gameSettings;

	std::mutex senderMutex;
	std::mutex receiverMutex;
};

#endif
