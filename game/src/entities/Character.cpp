#include "Character.h"

Character::Character(std::string name, Engine::Scene& scene)
	: Engine::Entity(name, scene)
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

void Character::Damage(int damage)
{
	health -= damage;
	m_HurtIndex = .2f;
}
