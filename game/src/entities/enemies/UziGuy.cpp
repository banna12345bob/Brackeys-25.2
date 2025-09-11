#include "UziGuy.h"

#include "../Bullet.h"
#include "../Player.h"

UziGuy::UziGuy(std::string name, Engine::Scene* scene, Player* player)
	: Enemy(name, scene, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 200;
}

void UziGuy::Attack(glm::vec2 dir)
{
	float randomAngleOffset = ((rand() % 1000) - 500) / 1000.f;
	Bullet* bullet = new Bullet(m_Scene, "test", m_player, 150, glm::atan(dir.x, dir.y) + randomAngleOffset);
	bullet->GetTransform()->position = GetTransform()->position;
	m_Scene->AddEntity(bullet);
}