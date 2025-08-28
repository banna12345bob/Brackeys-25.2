#pragma once

#include <engine.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum direction {
	north,
	east,
	south,
	west
};

class WaveFunctionCollapse
{
public:
	WaveFunctionCollapse(std::string filename);

	void CreateMap();

	void Render(Engine::Camera* camera);
private:
	void LoadTiles();
	json readJson(std::string filename);

	void Colapse(int x, int y);
	void CalcuateDomain(int x, int y);

	std::vector<int> FindSmallestDomain();


	struct Tile {
		Engine::Ref<Engine::Texture2D> texture;
		std::unordered_map<direction, std::vector<std::string>> validNeighbours;

		Tile() = default;
		Tile(const Tile&) = default;
		Tile(Engine::Ref<Engine::Texture2D> tilesheet, glm::vec2 texCoords, glm::vec2 tileSize, std::unordered_map<direction, std::vector<std::string>> validNeighbours)
		: validNeighbours(validNeighbours)
		{
			texture = Engine::SubTexture2D::CreateFromCoords(tilesheet, texCoords, tileSize);
		}
	};

	struct MapTile : public Tile {
		std::vector<std::string> domain;

		MapTile() = default;
		MapTile(const Tile& t) : Tile(t) {}
	};
private:
	json m_WFCData;

	std::unordered_map<std::string, Tile> m_Tiles;

	MapTile m_Map[10][10];
};

