#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <SFML/Graphics/Color.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <cstdint>

#include "../game/Direction.h"

struct GameSettings {

	uint32_t seed;
	uint8_t playerCount;
	std::vector<sf::Color> playerColors;
	float tickDuration;
	std::string mapName;
};

struct ClientInfo {

	int localPlayerCount;

	// array of (id, color) per player
	std::vector<std::pair<int, sf::Color>> infos;
};

struct InputList {

	int nbInput;
	
	std::vector<Direction> inputs;
};

sf::Packet& operator<<(sf::Packet& packet, const sf::Color& color);
sf::Packet& operator>>(sf::Packet& packet, sf::Color& color);

sf::Packet& operator<<(sf::Packet& packet, const GameSettings& settings);
sf::Packet& operator>>(sf::Packet& packet, GameSettings& settings);

sf::Packet& operator<<(sf::Packet& packet, const ClientInfo& info);
sf::Packet& operator>>(sf::Packet& packet, ClientInfo& info);

sf::Packet& operator<<(sf::Packet& packet, const InputList& inputs);
sf::Packet& operator>>(sf::Packet& packet, InputList& inputs);

#endif // !NETWORK_H_INCLUDED

