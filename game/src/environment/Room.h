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

	static Room* GenerateRoom(glm::vec2 position, Side entranceSide, Type type);
	static Room* GenerateHallway(Side entranceSide);
	static std::vector<Engine::BoundingBox> GenerateWallBoxes(int roomWidth, int roomHeight, bool hasGap, Side side);

private:
	int m_width, m_height;
	std::vector<Engine::BoundingBox> m_wallBoxes;
	Side m_entrance, m_exit;
	Type m_type;
};

