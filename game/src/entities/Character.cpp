#include "Character.h"

CharacterComponent::CharacterComponent(Engine::Scene* scene, Engine::Entity* entity)
	: m_Scene(scene), m_Entity(entity)
{
	m_Entity->AddComponent<Engine::SpriteRendererComponent>();
	m_Entity->AddComponent<Engine::VelocityComponent>();
}

void CharacterComponent::OnUpdate(Engine::Timestep ts)
{
	if (m_HurtIndex > 0)
	{
		m_Entity->GetComponent<Engine::SpriteRendererComponent>().colour = { 1, 0.75, 0.75, 1 };
		m_HurtIndex -= ts;
	} else {
		m_Entity->GetComponent<Engine::SpriteRendererComponent>().colour = { 1, 1, 1, 1 };
	}
}

bool CharacterComponent::Damage(int damage)
{
	if (invincible || m_HurtIndex > 0)
		return false;
	health -= damage;
	EG_TRACE(health);
	m_HurtIndex = .4f;
	return true;
}

void CharacterComponent::Move(glm::vec2 dir, int acceleration, int maxSpeed, Engine::Timestep ts) {

	// X axis
	if ((dir.x != 0 && m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x == 0) || dir.x * m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x > 0) {	// Same direction or just started moving
		if (abs(m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x) < maxSpeed * abs(dir.x)) {
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x += acceleration * dir.x * ts;
		}
		else {
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x = maxSpeed * dir.x;
		}
	}
	else if (dir.x == 0) {																			// Not moving on this axis
		if (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x != 0) {														// Need to decelerate
			int sign = (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x > 0) - (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x < 0);			// Hack I found on Stack Overflow
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x += acceleration * ts * -sign * 2.f;							// Decelerate faster than accelerate
			if (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x * sign < 0) {												// If we have gone past 0, set it to 0
				m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x = 0;
			}
		}
	}
	else {																							// Going in opposite direction
		m_Entity->GetComponent<Engine::VelocityComponent>().velocity.x = acceleration * dir.x * ts;
	}

	// Y axis
	if ((dir.y != 0 && m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y == 0) || dir.y * m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y > 0) {
		if (abs(m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y) < maxSpeed * abs(dir.y)) {
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y += acceleration * dir.y * ts;
		}
		else {
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y = maxSpeed * dir.y;
		}
	}
	else if (dir.y == 0) {
		if (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y != 0) {
			int sign = (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y > 0) - (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y < 0);
			m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y += acceleration * ts * -sign;
			if (m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y * sign < 0) {
				m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y = 0;
			}
		}
	}
	else {
		m_Entity->GetComponent<Engine::VelocityComponent>().velocity.y = acceleration * dir.y * ts;
	}
}