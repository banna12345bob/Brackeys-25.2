#include "WFC.h"

#include <random>

#include <imgui/imgui.h>

WaveFunctionCollapse::WaveFunctionCollapse(std::string filename, Engine::Scene* scene, glm::vec2 gridSize, glm::vec3 posOffset, glm::vec2 scaleMult)
	: m_MapWidth(gridSize.x), m_MapHeight(gridSize.y), m_PosOffset(posOffset), m_ScaleMult(scaleMult), m_Scene(scene)
{
	EG_PROFILE_FUNCTION();
	m_Offsets[north] = { 0, 1 };
	m_Offsets[east] = { 1, 0 };
	m_Offsets[south] = { 0, -1 };
	m_Offsets[west] = { -1, 0 };
    m_WFCData = readJson(filename);
	LoadTiles();
	CreateMap();
}

WaveFunctionCollapse::~WaveFunctionCollapse()
{
}

void WaveFunctionCollapse::OnImGuiRender()
{
	if (!showImGuiWindow)
		return;
	ImGui::Begin("WFC");
	for (int index = 0; index < map.size(); index++)
	{
		if (ImGui::TreeNode((std::to_string(index / m_MapWidth) + ", " + std::to_string(index % m_MapHeight) + ": " + std::to_string(map[index].domain.size())).c_str()))
		{
			std::string domains;
			for (auto domain : map[index].domain)
			{
				domains += domain + ", ";
				//if (ImGui::Button(domain.c_str()))
				//{
				//	map[index] = MapTile(tiles[domain]);
				//	map[index].domain.push_back(domain);
				//	m_NumDomain[index] = 1;
				//	for (int i = 0; i < map.size(); i++)
				//	{
				//		CalcuateDomain(i);
				//	}
				//	break;
				//}
			}
			ImGui::Text(domains.c_str());
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void WaveFunctionCollapse::CreateMap()
{
	EG_PROFILE_FUNCTION();
	for (int x = 0; x < m_MapWidth; x++)
	{
		for (int y = 0; y < m_MapHeight; y++)
		{
			MapTile tile;
			for (auto it = tiles.begin(); it != tiles.end(); it++)
			{
				tile.domain.push_back(it->first);
			}
			map.push_back(tile);
			m_NumDomain.push_back(tile.domain.size());
		}
	}	
}

void WaveFunctionCollapse::ColapseLoop()
{
	EG_PROFILE_FUNCTION();
	generating = true;
	m_mtx.lock();
	while (FindSmallestDomain() > -1)
	{
		Colapse(FindSmallestDomain());
	}
	m_mtx.unlock();
	generating = false;
	EG_INFO("World Gen Finished");
}

void WaveFunctionCollapse::Render(Engine::OrthographicCameraController* camera)
{
	EG_PROFILE_FUNCTION();
	Engine::Renderer2D::BeginScene(&camera->GetCamera());
	for (int i = 0; i < map.size(); i++)
	{
		glm::vec3 pos = glm::vec3(i / m_MapWidth * (m_WFCData["tileSize"]["x"] * m_ScaleMult.x), i % m_MapHeight * (m_WFCData["tileSize"]["y"] * m_ScaleMult.y), 0.f) + m_PosOffset;
		if (pos.y + m_WFCData["tileSize"]["y"] * m_ScaleMult.y <= camera->getBounds().Bottom - camera->getPosition().y)
			continue;
		if (pos.y - m_WFCData["tileSize"]["y"] * m_ScaleMult.y >= camera->getBounds().Top - camera->getPosition().y)
			continue;
		if (pos.x + m_WFCData["tileSize"]["x"] * m_ScaleMult.x <= camera->getBounds().Left - camera->getPosition().x)
			continue;
		if (pos.x - m_WFCData["tileSize"]["x"] * m_ScaleMult.x >= camera->getBounds().Right - camera->getPosition().x)
			continue;

		if (map[i].domain.size() == 1)
			Engine::Renderer2D::DrawQuad(pos, glm::vec2(m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"]) * m_ScaleMult, tiles[map[i].domain[0]].texture);
		else
			Engine::Renderer2D::DrawQuad(pos, glm::vec2(m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"]) * m_ScaleMult, { 1, 0, 1, 1 });
	}
	Engine::Renderer2D::EndScene();
}

void WaveFunctionCollapse::Colapse(int index)
{
	if (index == -1)
		return;

	if (map[index].domain.size() == 0) {
		EG_ERROR("{0}: ERROR domain is empty", index);
		Colapse(FindSmallestDomain());
		return;
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, map[index].domain.size() - 1);
	int tile = dist(rng);
	std::string node = map[index].domain[tile];
	SetTile(index, node);
}

void WaveFunctionCollapse::SetTile(int index, std::string tile)
{
	if (index > map.size() - 1)
		return;
	if (map[index].domain.size() == 1)
	{
		EG_WARN("Tried to set tile {0}: Tile already set", index);
		return;
	}

	map[index] = MapTile(tiles[tile]);
	map[index].domain.push_back(tile);
	m_NumDomain[index] = 1;

	if (glm::length(map[index].boundingBox.size()) != 0) {
		map[index].boundingBox.x = (glm::vec3(index / m_MapWidth * (m_WFCData["tileSize"]["x"] * m_ScaleMult.x) - map[index].boundingBox.width / 2, index % m_MapHeight * (m_WFCData["tileSize"]["y"] * m_ScaleMult.y) - map[index].boundingBox.height / 2, 0.f) + m_PosOffset).x;
		map[index].boundingBox.y = (glm::vec3(index / m_MapWidth * (m_WFCData["tileSize"]["x"] * m_ScaleMult.x) - map[index].boundingBox.width / 2, index % m_MapHeight * (m_WFCData["tileSize"]["y"] * m_ScaleMult.y) - map[index].boundingBox.height / 2, 0.f) + m_PosOffset).y;
		m_Scene->AddCollisionBox(map[index].boundingBox);
	}

	for (int x = -5; x <= 5; x++)
	{
		for (int y = -5; y <= 5; y++)
		{
			CalcuateDomain(index + x * m_MapWidth + y);
		}
	}
	//for (int i = 0; i < map.size(); i++)
	//{
	//	CalcuateDomain(i); 
	//}
}

void WaveFunctionCollapse::CalcuateDomain(int mapIndex)
{
	if (mapIndex > map.size() - 1 || mapIndex < 0)
		return;

	for (auto it = m_Offsets.begin(); it != m_Offsets.end(); it++)
	{
		std::vector<std::string> newDomain;
		glm::vec2 offset = { std::round(mapIndex / m_MapWidth), mapIndex % m_MapHeight };
		offset += it->second;
		if (offset.y > m_MapHeight - 1 || offset.y < 0 || offset.x > m_MapWidth - 1 || offset.x < 0)
			continue;

		for (int domainIndex = 0; domainIndex < map[mapIndex].domain.size(); domainIndex++)
		{
			for (auto& item : tiles[map[mapIndex].domain[domainIndex]].validNeighbours[it->first]) {
				if (!std::count(map[offset.x * m_MapWidth + offset.y].domain.begin(), map[offset.x * m_MapWidth + offset.y].domain.end(), item))
					continue;

				if (std::count(newDomain.begin(), newDomain.end(), item))
					continue;

				newDomain.push_back(item);
			}
		}
		if (newDomain.size() != 0)
			map[offset.x * m_MapWidth + offset.y].domain = newDomain;
		m_NumDomain[offset.x * m_MapWidth + offset.y] = map[offset.x * m_MapWidth + offset.y].domain.size();
	}
}

int WaveFunctionCollapse::FindSmallestDomain()
{
	int minIndex = -1;
	int min = std::accumulate(m_NumDomain.begin(), m_NumDomain.end(),
		m_NumDomain[0], [](int a, int b) {
			if (a <= 1 || b <= 1)
				return std::max(a, b);
			return std::min(a, b);
		});

	minIndex = std::find(m_NumDomain.begin(), m_NumDomain.end(), min) - m_NumDomain.begin();
	while (map[minIndex].domain.size() == 1)
	{
		minIndex++;
		if (minIndex == m_NumDomain.size()-2)
		{
			minIndex = -1;
			break;
		}
	}
	return minIndex;
}

void WaveFunctionCollapse::LoadTiles()
{
	EG_PROFILE_FUNCTION();
	Engine::Ref<Engine::Texture2D> tilesheet = Engine::Texture2D::Create(m_WFCData["tileSheet"]);
	glm::vec2 tileSize = { m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"] };
	for (json::iterator it = m_WFCData["tiles"].begin(); it != m_WFCData["tiles"].end(); ++it) {
		std::unordered_map<direction, std::vector<std::string>> validNeighbours;

		if (!m_WFCData["tiles"][it.key()]["validNeighbours"].is_array())
		{
			validNeighbours[north] = m_WFCData["tiles"][it.key()]["validNeighbours"]["north"].get<std::vector<std::string>>();
			validNeighbours[east] = m_WFCData["tiles"][it.key()]["validNeighbours"]["east"].get<std::vector<std::string>>();
			validNeighbours[south] = m_WFCData["tiles"][it.key()]["validNeighbours"]["south"].get<std::vector<std::string>>();
			validNeighbours[west] = m_WFCData["tiles"][it.key()]["validNeighbours"]["west"].get<std::vector<std::string>>();
		} else {
			validNeighbours[north] = m_WFCData["tiles"][it.key()]["validNeighbours"].get<std::vector<std::string>>();
			validNeighbours[east] = m_WFCData["tiles"][it.key()]["validNeighbours"].get<std::vector<std::string>>();
			validNeighbours[south] = m_WFCData["tiles"][it.key()]["validNeighbours"].get<std::vector<std::string>>();
			validNeighbours[west] = m_WFCData["tiles"][it.key()]["validNeighbours"].get<std::vector<std::string>>();
		}

		Engine::BoundingBox box = Engine::BoundingBox(0, 0, 0, 0);
		if (m_WFCData["tiles"][it.key()].contains("collision"))
			if (m_WFCData["tiles"][it.key()]["collision"])
				box = Engine::BoundingBox(0, 0, m_WFCData["tileSize"]["x"] * m_ScaleMult.x, m_WFCData["tileSize"]["y"] * m_ScaleMult.y);
		tiles[it.key()] = Tile(tilesheet, { it.value()["texCoords"]["x"], it.value()["texCoords"]["y"] }, tileSize, validNeighbours, box);
	}
}

json WaveFunctionCollapse::readJson(std::string filename)
{
	std::string fileExtension = filename.substr(filename.find_last_of(".") + 1);
	EG_CORE_ASSERT(fileExtension == "json", "File is not json");
	std::ifstream f(filename);
	if (!(f)) {
		return false;
	}
	json data = json::parse(f);
	return data;
}


