#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Network.hpp>
#include <SFML/Graphics/Color.hpp>

class Client {

public:
	Client();

	bool Start(const std::string& ip, int port, const std::vector<sf::Color>& playersColor);

private:

	sf::TcpSocket socket;
};

#endif
