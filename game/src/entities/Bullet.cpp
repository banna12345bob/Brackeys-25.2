#include "Bullet.h"

#include "Character.h"

#include <algorithm>

Bullet::Bullet(Engine::Scene& scene, std::string name, Engine::UUID playerUUID, float initalAngle) 
	: Engine::Entity(name, scene), m_PlayerUUID(playerUUID), theta(initalAngle), speed(500)
{
	GetTransform()->scale = { 16.f, 16.f };
	GetSpriteRenderer()->colour = { 1, 0, 0, 1 };
}

void Bullet::OnUpdate(Engine::Timestep ts)
{
	Character* player = (Character*)m_Scene.GetEntity(m_PlayerUUID);
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

	GetVelocity()->velocity.x = speed * glm::sin(theta) * ts;
	GetVelocity()->velocity.y = speed * glm::cos(theta) * ts;

	if (OverLappingWithEntity(player)) {
		player->Damage(1);
		this->active = false;
		this->hide = true;
	}

	Engine::Entity::OnUpdate(ts);
}

bool Bullet::OverLappingWithEntity(Engine::Entity* entity) {
	return (glm::abs(GetTransform()->position.x - entity->GetTransform()->position.x) * 2 < (GetTransform()->scale.x + entity->GetTransform()->scale.x)) &&
		(glm::abs(GetTransform()->position.y - entity->GetTransform()->position.y) * 2 < (GetTransform()->scale.y + entity->GetTransform()->scale.y));
}
