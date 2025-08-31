#include "PistolGuy.h"
#include "../Bullet.h"
#include "../Player.h"

PistolGuy::PistolGuy(std::string name, Engine::Scene& scene, Player& player)
	: Enemy(name, scene, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 1200;
}

void PistolGuy::Attack(glm::vec2 dir)
{
	Bullet* bullet = new Bullet(m_Scene, "test", m_player.EntityUUID, 150, glm::atan(dir.x, dir.y));
	bullet->GetTransform()->position = GetTransform()->position;
	m_Scene.AddEntity(bullet);
}