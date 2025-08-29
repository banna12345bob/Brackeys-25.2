#include "WFC.h"

#include <time.h>
#include <random>

#include <imgui/imgui.h>

WaveFunctionCollapse::WaveFunctionCollapse(std::string filename)
{
	EG_PROFILE_FUNCTION();
	m_Offsets[north] = { 0, 1 };
	m_Offsets[east] = { 1, 0 };
	m_Offsets[south] = { 0, -1 };
	m_Offsets[west] = { -1, 0 };
    m_WFCData = readJson(filename);
	LoadTiles();
}

WaveFunctionCollapse::~WaveFunctionCollapse()
{
	m_Map = std::vector<MapTile>();
	m_Tiles = std::unordered_map<std::string, Tile>();
}

void WaveFunctionCollapse::OnImGuiRender()
{
	return;
	ImGui::Begin("WFC");
	for (int index = 0; index < m_Map.size(); index++)
	{
		if (ImGui::TreeNode((std::to_string(index / m_MapWidth) + ", " + std::to_string(index % m_MapHeight) + ": " + std::to_string(m_Map[index].domain.size())).c_str()))
		{
			std::string domains = "";
			for (auto& domain : m_Map[index].domain)
			{
				domains += domain + ", ";
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
			for (auto it = m_Tiles.begin(); it != m_Tiles.end(); it++)
			{
				tile.domain.push_back(it->first);
			}
			m_Map.push_back(tile);
		}
	}

	//glm::vec2 offset = { 2, 2 };
	//for (int x = 0; x < 5; x++)
	//{
	//	for (int y = 0; y < 5; y++)
	//	{
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y] = MapTile();
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y].domain.push_back("dirtMiddle");
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y].domain.push_back("dirtMiddle1");
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y].domain.push_back("dirtMiddle2");
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y].domain.push_back("dirtMiddle3");
	//		m_Map[((x + offset.x) * m_MapWidth) + y + offset.y].domain.push_back("dirtMiddle4");
	//	}
	//}

	CalcuateDomain();
	Colapse(FindSmallestDomain());
}

void WaveFunctionCollapse::Render(Engine::Camera* camera)
{
	EG_PROFILE_FUNCTION();
	Engine::Renderer2D::BeginScene(camera);
	for (int i = 0; i < m_Map.size(); i++)
	{
		if (m_Map[i].texture)
			Engine::Renderer2D::DrawQuad({ i / m_MapWidth * m_WFCData["tileSize"]["x"], i % m_MapHeight * m_WFCData["tileSize"]["y"], 0.f }, { m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"] }, m_Map[i].texture);
		else
			Engine::Renderer2D::DrawQuad({ i / m_MapWidth * m_WFCData["tileSize"]["x"], i % m_MapHeight * m_WFCData["tileSize"]["y"], 0.f }, { m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"] }, { 1, 0, 1, 1 });
	}
	Engine::Renderer2D::EndScene();
}

void WaveFunctionCollapse::Colapse(int index)
{
	EG_PROFILE_FUNCTION();
	if (index == -1)
		return;

	if (m_Map[index].domain.size() == 0) {
		EG_ERROR("{0}: ERROR domain is empty", index);
		Colapse(FindSmallestDomain());
		return;
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_Map[index].domain.size() - 1);
	int tile = dist(rng);
	std::string node = m_Map[index].domain[tile];
	m_Map[index] = MapTile(m_Tiles[node]);
	m_Map[index].domain.push_back(node);

	CalcuateDomain();

	Colapse(FindSmallestDomain());
	return;
}	

void WaveFunctionCollapse::CalcuateDomain()
{
	EG_PROFILE_FUNCTION();
	for (int mapIndex = 0; mapIndex < m_Map.size(); mapIndex++)
	{
		for (auto it = m_Offsets.begin(); it != m_Offsets.end(); it++)
		{
			std::vector<std::string> newDomain;
			glm::vec2 offset = { std::round(mapIndex / m_MapWidth), mapIndex % m_MapHeight };
			offset += it->second;
			if (offset.y > m_MapHeight - 1 || offset.y < 0 || offset.x > m_MapWidth - 1 || offset.x < 0)
				continue;

			for (auto& domainItem : m_Map[mapIndex].domain)
			{
				for (auto& item : m_Tiles[domainItem].validNeighbours[it->first]) {
					if (!std::count(m_Map[offset.x * m_MapWidth + offset.y].domain.begin(), m_Map[offset.x * m_MapWidth + offset.y].domain.end(), item))
						continue;

					if (std::count(newDomain.begin(), newDomain.end(), item))
						continue;

					newDomain.push_back(item);
				}
			}
			if (newDomain.size() != 0)
				m_Map[offset.x * m_MapWidth + offset.y].domain = newDomain;
		}
	}
}

int WaveFunctionCollapse::FindSmallestDomain()
{
	EG_PROFILE_FUNCTION();
	MapTile* min = new MapTile();
	for (int i = 0; i < 100; i++)
	{
		min->domain.push_back(std::to_string(i));
	}
	int minIndex = -1;

	for (int i = 0; i < m_Map.size(); i++)
	{
		if (min->texture) {
			min = new MapTile();
			for (int i = 0; i < 100; i++)
			{
				min->domain.push_back(std::to_string(i));
			}
			continue;
		}

		if (m_Map[i].domain.size() < min->domain.size() && !m_Map[i].texture && m_Map[i].domain.size() != 0)
		{
			minIndex = i;
			min = &m_Map[i];
		}
	}
	return minIndex;
}

void WaveFunctionCollapse::LoadTiles()
{
	EG_PROFILE_FUNCTION();
	Engine::Ref<Engine::Texture2D> tilesheet = Engine::Texture2D::Create(m_WFCData["tilesheet"]);
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

		m_Tiles[it.key()] = Tile(tilesheet, { it.value()["texCoords"]["x"], it.value()["texCoords"]["y"] }, tileSize, validNeighbours);
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


