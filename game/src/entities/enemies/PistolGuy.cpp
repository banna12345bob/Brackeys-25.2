#include "PistolGuy.h"
#include "../Bullet.h"

PistolGuyComponent::PistolGuyComponent(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player)
	: EnemyComponenet(scene, entity, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 1200;
}

void PistolGuyComponent::Attack(glm::vec2 dir)
{
	Engine::Entity bullet = m_Scene->AddEntity("bullet");
	//bullet.AddComponent<BulletComponenet>(m_Scene, &bullet, m_player, 150, glm::atan(dir.x, dir.y), 2);
	bullet.GetComponent<Engine::TransformComponent>().position = m_Entity->GetComponent<Engine::TransformComponent>().position;
}