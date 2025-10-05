#include "UziGuy.h"

#include "../Bullet.h"

UziGuyComponenet::UziGuyComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player)
	: EnemyComponenet(scene, entity, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 200;
}

void UziGuyComponenet::Attack(glm::vec2 dir)
{
	float randomAngleOffset = ((rand() % 1000) - 500) / 1000.f;
	Engine::Entity bullet = m_Scene->AddEntity("bullet");
	//bullet.AddComponent<BulletComponenet>(m_Scene, &bullet, m_player, 150, glm::atan(dir.x, dir.y) + randomAngleOffset, 2);
	bullet.GetComponent<Engine::TransformComponent>().position = m_Entity->GetComponent<Engine::TransformComponent>().position;
}