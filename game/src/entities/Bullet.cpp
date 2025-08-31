#include "Bullet.h"

#include "Character.h"

#include <algorithm>

Bullet::Bullet(Engine::Scene& scene, std::string name, Player* player, int speed, float initalAngle, float lifetime)
	: Engine::Entity(name, scene), m_Player(player), theta(initalAngle), speed(speed), lifetime(lifetime)
{
	GetTransform()->scale = { 16.f, 16.f };
	GetSpriteRenderer()->colour = { 1, 0, 0, 1 };

	GetVelocity()->velocity.x = speed * glm::sin(theta);
	GetVelocity()->velocity.y = speed * glm::cos(theta);
}

void Bullet::OnUpdate(Engine::Timestep ts)
{
	if (lifetime < 0)
	{
		this->active = false;
		this->hide = true;
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
		if (m_Player->Damage(1)) {
			this->active = false;
			this->hide = true;
		}
	}
	
	lifetime -= ts;
	Engine::Entity::OnUpdate(ts);
}

bool Bullet::OverLappingWithEntity(Engine::Entity* entity) {
	return (glm::abs(GetTransform()->position.x - entity->GetTransform()->position.x) * 2 < (GetTransform()->scale.x + entity->GetTransform()->scale.x)) &&
		(glm::abs(GetTransform()->position.y - entity->GetTransform()->position.y) * 2 < (GetTransform()->scale.y + entity->GetTransform()->scale.y));
}
