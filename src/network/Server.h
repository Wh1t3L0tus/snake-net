#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <memory>
#include <thread>
#include <mutex>
#include <SFML/Network.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../game/Direction.h"
#include "Network.h"

struct Player {

	char id;
	sf::Color color;
	Direction direction;
};

struct ClientStruct {

	std::shared_ptr<sf::TcpSocket> socket;
	std::vector<Player> players;
	std::thread listenerThread;
};

class Server {

public:

	Server(const std::string& ip, int port, int playerCount, float tickDuration, std::string mapName);

	~Server();


	// Start server :
	//   - accept 'playerCount' players
	//   - send GameSettings to all players
	//   - send StartGame message to all players
	//   - start game loop
	//
	// Returns true on success, false otherwise (with info written on std::cerr)
	bool start();


private:

	bool lobbyLoop();

	void gameLoop();

	/** Accepts a client
	 * @param client : new accepted client
	 * @param connectedPlayers : number of connected players
	 * @returns true on success
	 */
	bool onAcceptClient(std::shared_ptr<sf::TcpSocket> client, int& newPlayersCount);

	GameSettings createGameSettings();

	void startListenerThreads();

	void fillInputList(InputList& inputList);

	bool broadcastInputList(const InputList& inputList);

	sf::TcpListener listener;

	std::string ip;
	int port;
	
	int playerCount;
	float tickDuration;
	std::string mapName;

	std::vector<ClientStruct> clients;

	std::mutex playerMutex;
};


#endif // !SERVER_H_INCLUDED

