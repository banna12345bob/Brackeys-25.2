#pragma once

#include <vector>
#include "engine.h"

class Room
{
public:

	static inline const int TileSize = 16;
	static inline const int EntraceWidth = 3;

	enum Type {
		START,
		END,
		COMBAT,
		ITEM_UPGRADE,
		PASSAGE
	};

	enum Side {
		LEFT = 0,
		RIGHT = 1,
		TOP = 2,
		BOTTOM = 3
	};

	Room(int width, int height, std::vector<Engine::BoundingBox> wallBoxes, Side entrance, Side exit, Type type);
	std::vector<Engine::BoundingBox> GetBoundingBoxes();
	void OnRender();

	static inline Room* GenerateRoom(glm::vec2 position, Side entranceSide, Type type);
	static inline Room* GenerateHallway(Side entranceSide);
	static inline std::vector<Engine::BoundingBox> GetWallBoxes(int roomWidth, int roomHeight, bool hasGap, Side side);

private:
	Type m_type;
	int m_width, m_height;
	std::vector<Engine::BoundingBox> m_wallBoxes;
	std::vector<Engine::BoundingBox> m_entrances;
};

