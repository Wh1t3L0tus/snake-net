#include "MapLoader.h"

#include <fstream>
#include <string>

const std::string PATH_MAPS = "Assets/maps/";

const char SPAWNER_E = '>';
const char SPAWNER_W = '<';
const char SPAWNER_N = 'A';
const char SPAWNER_S = 'V';
const char OBSTACLE = 'X';
const char VOID = ' ';

bool MapLoader::LoadMap(const std::string& filename, Map& map) {

	std::ifstream file(PATH_MAPS + filename);

	if (!file.is_open()) {
		return false;
	}

	int height = 0;
	int width = -1;
	std::string line;

	while (std::getline(file, line)) {

		if (width == -1) {
			width = line.size();
		}

		for (int i = 0; i < line.size(); i++) {
			
			char currentTile = line[i];

			switch (currentTile)
			{
			case SPAWNER_E:
				map.spawners.push_back(Spawner{ sf::Vector2i(i, height), Direction::RIGHT });
				map.tiles.push_back(CellState::EMPTY);
				break;
			case SPAWNER_W:
				map.spawners.push_back(Spawner{ sf::Vector2i(i, height), Direction::LEFT });
				map.tiles.push_back(CellState::EMPTY);
				break;
			case SPAWNER_N:
				map.spawners.push_back(Spawner{ sf::Vector2i(i, height), Direction::UP });
				map.tiles.push_back(CellState::EMPTY);
				break;
			case SPAWNER_S:
				map.spawners.push_back(Spawner{ sf::Vector2i(i, height), Direction::DOWN });
				map.tiles.push_back(CellState::EMPTY);
				break;
			case OBSTACLE:
				map.tiles.push_back(CellState::WALL);
				break;
			case VOID:
				map.tiles.push_back(CellState::EMPTY);
				break;
			default:
				return false;
				break;
			}
		}

		height++;
	}

	map.width = width;
	map.height = height;

	file.close();

	return true;
}