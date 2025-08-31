#include "Character.h"

Character::Character(std::string name, Engine::Scene& scene, int health)
	: Engine::Entity(name, scene), health(health)
{
}

void Character::OnUpdate(Engine::Timestep ts)
{
	if (m_HurtIndex > 0)
	{
		GetSpriteRenderer()->colour = { 1, 0.75, 0.75, 1 };
		m_HurtIndex -= ts;
	} else {
		GetSpriteRenderer()->colour = { 1, 1, 1, 1 };
	}

	Entity::OnUpdate(ts);
}

bool Character::Damage(int damage)
{
	if (invincible || m_HurtIndex > 0)
		return false;
	health -= damage;
	EG_TRACE(health);
	m_HurtIndex = .4f;
	return true;
}
