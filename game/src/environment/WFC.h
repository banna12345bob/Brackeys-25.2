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
	WaveFunctionCollapse(std::string filename, Engine::Scene* scene, glm::vec2 gridSize, glm::vec3 posOffset = glm::vec3(0.f), glm::vec2 scaleMult = glm::vec2(1.f));

	~WaveFunctionCollapse();

	void CreateMap();
	void ColapseLoop();
	void SetTile(int index, std::string tile);

	void Render(Engine::OrthographicCameraController* camera);

	void OnImGuiRender();

	struct Tile {
		Engine::Ref<Engine::Texture2D> texture;
		std::unordered_map<direction, std::vector<std::string>> validNeighbours;
		Engine::BoundingBox boundingBox;

		Tile() = default;
		Tile(const Tile&) = default;
		Tile(const Engine::Ref<Engine::Texture2D> tilesheet, const glm::vec2 texCoords, const glm::vec2 tileSize, const std::unordered_map<direction, std::vector<std::string>> validNeighbours, Engine::BoundingBox box)
		: validNeighbours(validNeighbours), boundingBox(box)
		{
			texture = Engine::SubTexture2D::CreateFromCoords(tilesheet, texCoords, tileSize);
		}
	};

	struct MapTile : public Tile {
		std::vector<std::string> domain;

		MapTile() = default;
		MapTile(const Tile& t) : Tile(t) {}
	};

	std::vector<MapTile> map;
	std::unordered_map<std::string, Tile> tiles;
	bool generating = false;
	bool showImGuiWindow = false;
private:
	void LoadTiles();
	json readJson(std::string filename);
	void Colapse(int index);
	int FindSmallestDomain();

	void CalcuateDomain(int i);
private:
	json m_WFCData;

	glm::vec3 m_PosOffset;
	glm::vec2 m_ScaleMult;

	int m_MapWidth;
	int m_MapHeight;

	std::mutex m_mtx;

	std::unordered_map<direction, glm::vec2> m_Offsets;
	std::vector<int> m_NumDomain;

	Engine::Scene* m_Scene;
};

