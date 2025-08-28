#include "Room.h"
#include <random>

Room::Room(int width, int height, std::vector<Engine::BoundingBox> wallBoxes, Side entrance, Side exit, Type type)
	: m_width(width), m_height(height), m_wallBoxes(wallBoxes), m_entrance(entrance), m_exit(exit), m_type(type)
{

}

Room* Room::GenerateRoom(glm::vec2 position, Side entranceSide, Type type) {
	int width = 5 + rand() % 5;
	int height = 5 + rand() % 5;

	Side exitSide = (Side)(rand() % 4);

	// If start or end, make entrance and exit the same so that the for loop below doesn't get messed up when checking of the wall has a gap
	if (type == START) {
		entranceSide = exitSide;
	}
	else if (type == END) {
		exitSide = entranceSide;
	}
	else if (exitSide == entranceSide) {
			exitSide = (Side)(((int)entranceSide + 2) % 4);
	}
	
	std::vector<Engine::BoundingBox> wallBoxes;

	for (int i = 0; i < 4; i++) {
		Side side = (Side)i;
		bool hasGap = entranceSide == side || exitSide == side;
		std::vector<Engine::BoundingBox> boxes;// = GenerateWallBoxes(width, height, hasGap, side);
		wallBoxes.insert(wallBoxes.end(), boxes.begin(), boxes.end());
	}

	// At this point the position of the wall bounding boxes are relative to the centre of the room, so rectify that
	for (auto it = wallBoxes.begin(); it != wallBoxes.end(); it++) {
		it->x += position.x;
		it->y += position.y;
	}

	Room* room = new Room(width, height, wallBoxes, entranceSide, exitSide, type);

	return room;
}

Room* Room::GenerateHallway(Side entranceSide) {
	int width = 5 + rand() % 5;
	if (width % 2 == 0) width++;

	int height = 5 + rand() % 5;
	if (height % 2 == 0) height++;

	Side exitSide = (Side)(((int)entranceSide + 2) % 4);

	// TODO: Generate the bounding boxes

	return nullptr;
}

// Doesn't work but it won't be used anyway I don't think
//std::vector<Engine::BoundingBox> Room::GenerateWallBoxes(int roomWidth, int roomHeight, bool hasGap, Side side) {
//	std::vector<Engine::BoundingBox> boxes;
//	Engine::BoundingBox box;
//
//	// In px
//	int halfRoomWidth = roomWidth * TileSize / 2;
//	int halfRoomHeight = roomHeight * TileSize / 2;
//	
//	// Overlap at corners because it is a little simpler
//	switch (side) {
//	case LEFT:
//		box.x = -halfRoomWidth - TileSize;
//		box.y = -halfRoomHeight - TileSize;
//		break;
//	case TOP:
//		box.x = -halfRoomWidth - TileSize;
//		box.y = halfRoomHeight + TileSize;
//		break;
//	case RIGHT:
//		box.x = halfRoomWidth + TileSize;
//		box.y = -halfRoomHeight - TileSize;
//		break;
//	case BOTTOM:
//		box.x = halfRoomWidth + TileSize;
//		box.y = -halfRoomWidth - TileSize;
//		break;
//	}
//
//	if ((int)side <= 2) {
//		box.width = TileSize;
//		int heightInTiles = roomHeight + 2;
//
//		Engine::BoundingBox otherBox;
//		if (hasGap) {
//			box.height = (heightInTiles - 3) / 2;
//			box.height *= TileSize;
//
//			Engine::BoundingBox otherBox = Engine::BoundingBox(box.x, box.y + box.height + (3 * TileSize), TileSize, box.height);
//
//			boxes.push_back(otherBox);
//		}
//		else {
//			box.height = heightInTiles * TileSize;
//		}
//	}
//	else {
//		box.height = TileSize;
//		int widthInTiles = roomWidth + 2;
//
//		Engine::BoundingBox otherBox;
//		if (hasGap) {
//			box.width = (widthInTiles - 3) / 2;
//			box.width *= TileSize;
//
//			Engine::BoundingBox otherBox = Engine::BoundingBox(box.x + box.width + (3 * TileSize), box.y, box.width, TileSize);
//
//			boxes.push_back(otherBox);
//		}
//		else {
//			box.width = widthInTiles * TileSize;
//		}
//	}
//	
//	boxes.push_back(box);
//
//	return boxes;
//}

std::vector<Engine::BoundingBox> Room::GetBoundingBoxes() {
	return m_wallBoxes;
}

void Room::OnRender() {

}