#include "Bullet.h"

#include "Player.h"

BulletComponenet::BulletComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player, int speed, float initalAngle, float lifetime)
	: m_Scene(scene), m_Entity(entity->getUUID()), m_Player(player), theta(initalAngle), speed(speed), lifetime(lifetime)
{
	m_Scene->GetEntity(m_Entity).AddComponent<Engine::SpriteRendererComponent>();
	m_Scene->GetEntity(m_Entity).AddComponent<Engine::VelocityComponent>();

	m_Scene->GetEntity(m_Entity).GetComponent<Engine::TransformComponent>().scale = { 16.f, 16.f };
	m_Scene->GetEntity(m_Entity).GetComponent<Engine::SpriteRendererComponent>().colour = { 1, 0, 0, 1 };

	m_Scene->GetEntity(m_Entity).GetComponent<Engine::VelocityComponent>().velocity.x = speed * glm::sin(theta);
	m_Scene->GetEntity(m_Entity).GetComponent<Engine::VelocityComponent>().velocity.y = speed * glm::cos(theta);
}

void BulletComponenet::OnUpdate(Engine::Timestep ts)
{
	if (lifetime < 0 && lifetime != -1.f)
	{
		m_Scene->GetEntity(m_Entity).GetComponent<Engine::VelocityComponent>().velocity = { 0.f, 0.f, 0.f };
		m_Scene->GetEntity(m_Entity).GetComponent<Engine::MetaDataComponent>().hide = true;
		m_Scene->RemoveEntity(m_Scene->GetEntity(m_Entity));
		return;
	}
	//m_Theta = glm::atan((GetTransform()->position.y - player->GetTransform()->position.y) / (GetTransform()->position.x - player->GetTransform()->position.x));

	//float theta = glm::atan((GetTransform()->position.y - player->GetTransform()->position.y) / (GetTransform()->position.x - player->GetTransform()->position.x));

	// tan(theta) = (pos.y - posP.y) / (pos.x - posP.x)
	// (pos.x - posP.x) * tan(theta) = pos.y - posP.y
	// (tan(theta) * pos.x) - (tan(theta) * posP.x) = pos.y - posP.y
	// tan(theta) * pos.x = (pos.y - posP.y) + (tan(theta) * posP.x)
	// pos.x = (pos.y - posP.y) + (tan(theta) * posP.x) / tan(theta)
	// pos.x = (pos.y - posP.y) / tan(theta) + posP.x
	//GetVelocity()->velocity.x = (GetTransform()->position.y - player->GetTransform()->position.y) / glm::tan(-theta) + player->GetTransform()->position.x * ts;

	// theta = archtan((pos.y - posP.y) / (pos.x - posP.x))
	// tan(theta) = (pos.y - posP.y) / (pos.x - posP.x)
	// (pos.x - posP.x) * tan(theta) = pos.y - posP.y
	// pos.y = (pos.x - posP.x) * tan(theta) + posP.y
	//GetVelocity()->velocity.y = (GetTransform()->position.x - player->GetTransform()->position.x) * glm::tan(-theta) + player->GetTransform()->position.y * ts;

	if (OverLappingWithEntity(m_Player)) {
		//if (m_Player->GetComponent<PlayerComponent>().Damage(1)) {
		//	m_Scene->GetEntity(m_Entity).GetComponent<Engine::VelocityComponent>().velocity = { 0.f, 0.f, 0.f };
		//	m_Scene->GetEntity(m_Entity).GetComponent<Engine::MetaDataComponent>().hide = true;
		//	m_Scene->RemoveEntity(m_Scene->GetEntity(m_Entity));
		//}
	}
	
	if (lifetime != -1.f)
	{
		lifetime -= ts;
	}
}

bool BulletComponenet::OverLappingWithEntity(Engine::Entity* entity) {
	bool check1 = glm::abs(m_Scene->GetEntity(m_Entity).GetComponent<Engine::TransformComponent>().position.x - entity->GetComponent<Engine::TransformComponent>().position.x) * 2
		< (m_Scene->GetEntity(m_Entity).GetComponent<Engine::TransformComponent>().scale.x + entity->GetComponent<Engine::TransformComponent>().scale.x);
	bool check2 = glm::abs(m_Scene->GetEntity(m_Entity).GetComponent<Engine::TransformComponent>().position.y - entity->GetComponent<Engine::TransformComponent>().position.y) * 2
		< (m_Scene->GetEntity(m_Entity).GetComponent<Engine::TransformComponent>().scale.y + entity->GetComponent<Engine::TransformComponent>().scale.y);
	return check1 && check2;
}
