#include "WFC.h"

#include <time.h>
#include <random>

WaveFunctionCollapse::WaveFunctionCollapse(std::string filename)
{
    m_WFCData = readJson(filename);
	LoadTiles();
}

void WaveFunctionCollapse::CreateMap()
{
	for (int x = 0; x < sizeof(m_Map) / sizeof(m_Map[0]); x++)
	{
		for (int y = 0; y < sizeof(m_Map[0]) / sizeof(MapTile); y++)
		{
			MapTile tile;
			for (auto it = m_Tiles.begin(); it != m_Tiles.end(); )
			{
				tile.domain.push_back(it->first);
				m_Map[x][y] = tile;

				it++;
			}
		}
	}

	//m_Map[5][5] = MapTile(m_Tiles["dirtMiddle"]);
	//CalcuateDomain(5, 5);

	std::vector<int> smallestDomain = FindSmallestDomain();

	Colapse(smallestDomain[0], smallestDomain[1]);
	for (int x = 0; x < sizeof(m_Map) / sizeof(m_Map[0]); x++)
		for (int y = 0; y < sizeof(m_Map[0]) / sizeof(MapTile); y++)
			if (!m_Map[x][y].texture)
				EG_ERROR("{0},{1}", x, y);
}

void WaveFunctionCollapse::Render(Engine::Camera* camera)
{
	Engine::Renderer2D::BeginScene(camera);
	for (int x = 0; x < sizeof(m_Map) / sizeof(m_Map[0]); x++)
	{
		for (int y = 0; y < sizeof(m_Map[0]) / sizeof(MapTile); y++)
		{
			if (m_Map[x][y].texture)
				Engine::Renderer2D::DrawQuad({ x * m_WFCData["tileSize"]["x"], y * m_WFCData["tileSize"]["y"], 0.f }, { m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"] }, m_Map[x][y].texture);
			else
				Engine::Renderer2D::DrawQuad({ x * m_WFCData["tileSize"]["x"], y * m_WFCData["tileSize"]["y"], 0.f }, { m_WFCData["tileSize"]["x"], m_WFCData["tileSize"]["y"] }, { 1, 0, 1, 1 });

			Engine::Renderer2D::DrawQuad({ x * m_WFCData["tileSize"]["x"], y * m_WFCData["tileSize"]["y"], 0.1f }, { 1, 1 }, { 0, 0,0,1 });
		}
	}
	Engine::Renderer2D::DrawQuad({ 5 * m_WFCData["tileSize"]["x"], 5 * m_WFCData["tileSize"]["y"], 0.15f }, { 1, 1 }, { 0, 1, 1, 1 });
	Engine::Renderer2D::EndScene();
}

void WaveFunctionCollapse::Colapse(int x, int y)
{
	if (x == -1 || y == -1)
		return;

	if (m_Map[x][y].domain.size() == 1) 
		m_Map[x][y] = MapTile(m_Tiles[m_Map[x][y].domain[0]]);
	if (m_Map[x][y].domain.size() == 0) {
		Colapse(FindSmallestDomain()[0], FindSmallestDomain()[1]);
		return;
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_Map[x][y].domain.size() - 1);
	int tile = dist(rng);

	EG_TRACE("{0},{1}: {2}", x, y, m_Map[x][y].domain[tile]);

	m_Map[x][y] = MapTile(m_Tiles[m_Map[x][y].domain[tile]]);

	CalcuateDomain(x, y);

	std::vector<int> minDomain = FindSmallestDomain();

	Colapse(minDomain[0], minDomain[1]);
	return;
}	

void WaveFunctionCollapse::CalcuateDomain(int x, int y)
{
	for (auto it = m_Map[x][y].validNeighbours.begin(); it != m_Map[x][y].validNeighbours.end(); )
	{
		int nX = x;
		int nY = y;
		if (it->first == north && nY != sizeof(m_Map[0]) / sizeof(MapTile) - 1)
			nY++; 
		else if (it->first == south && nY != 0)
			nY--;
		else if (it->first == east && nX != sizeof(m_Map) / sizeof(m_Map[0]) - 1)
			nX++;
		else if (it->first == west && nX != 0)
			nX--;
		else {
			it++;
			continue;
		}

		std::vector<std::string> newDomain;
		for (auto& tile : it->second)
		{
			if (std::find(m_Map[nX][nY].domain.begin(), m_Map[nX][nY].domain.end(), tile) != m_Map[nX][nY].domain.end())
			{
				if (m_Tiles.contains(tile))
					// TODO: Proergate the new domain out
					newDomain.push_back(tile);
			}
		}

		m_Map[nX][nY].domain = newDomain;

		it++;
	}
}

std::vector<int> WaveFunctionCollapse::FindSmallestDomain()
{
	MapTile* min = new MapTile();
	for (int i = 0; i < 100; i++)
	{
		min->domain.push_back(std::to_string(i));
	}
	std::vector<int> minIndex = std::vector<int>();
	minIndex.push_back(-1);
	minIndex.push_back(-1);

	for (int x = 0; x < sizeof(m_Map) / sizeof(m_Map[0]); x++)
	{
		for (int y = 0; y < sizeof(m_Map[0]) / sizeof(MapTile); y++)
		{
			if (m_Map[x][y].domain.size() < min->domain.size() && m_Map[x][y].domain.size() != 0)
			{
				minIndex[0] = x;
				minIndex[1] = y;
				min = &m_Map[x][y];
			}
		}
	}
	return minIndex;
}

void WaveFunctionCollapse::LoadTiles()
{
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


