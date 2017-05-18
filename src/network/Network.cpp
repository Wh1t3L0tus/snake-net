#include "Network.h"

sf::Packet& operator<<(sf::Packet& packet, const sf::Color& c) {

	return packet << c.r << c.g << c.b << c.a;
}

sf::Packet& operator>>(sf::Packet& packet, sf::Color& c) {

	return packet >> c.r >> c.g >> c.b >> c.a;
}

sf::Packet& operator<<(sf::Packet& packet, const GameSettings& s) {

	packet << s.playerCount << s.seed << s.tickDuration << s.mapName;
	
	for (char i = 0; i < s.playerCount; i++) {

		packet << s.playerColors[i];
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, GameSettings& s) {
	
	packet >> s.playerCount >> s.seed >> s.tickDuration >> s.mapName;

	for (char i = 0; i < s.playerCount; i++) {

		packet >> s.playerColors[i];
	}

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const ClientInfo& info) {

	packet << info.localPlayerCount;

	for (char i = 0; i < info.infos.size(); i++) {

		packet << info.infos[i].first << info.infos[i].second;
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, ClientInfo& info) {

	packet >> info.localPlayerCount;

	for (char i = 0; i < info.localPlayerCount; i++) {
		int id;
		sf::Color color;
		
		packet >> id >> color;

		info.infos.push_back(std::pair<int, sf::Color>(id, color));
	}

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const InputList& inputs) {

	packet << inputs.nbInput;

	for (char i = 0; i < inputs.nbInput; i++) {

		packet << static_cast<sf::Int8>(inputs.inputs[i]);
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, InputList& inputs) {

	packet >> inputs.nbInput;

	for (char i = 0; i < inputs.nbInput; i++) {
		
		sf::Int8 data;
		packet >> data;
		inputs.inputs[i] = static_cast<Direction>(data);
	}

	return packet;
}