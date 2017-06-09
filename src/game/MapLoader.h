#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <SFML/System/Vector2.hpp>

#include <vector>
#include "CellState.h"
#include "Direction.h"

struct Spawner {
	sf::Vector2i coords;
	Direction direction;
};

struct Map {
	std::vector<Spawner> spawners;
	std::vector<CellState> tiles;
	int width;
	int height;
};




class MapLoader {

public:

	static bool LoadMap(const std::string& filename, Map& map);

};

#endif // !MAP_LOADER_H

