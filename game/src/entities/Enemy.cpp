#include "Enemy.h"


EnemyComponenet::EnemyComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player)
	: CharacterComponent(scene, entity),
	m_acceleration(400),
	m_maxSpeed(60),
	m_attackRange(128),
	m_attackSpeed(600),
	m_attackTimer(2000),
	m_player(player),
	m_idleTimer(0),
	m_randDir(glm::vec2(0))
{
	m_Entity->GetComponent<Engine::TransformComponent>().scale = { 16.f, 16.f };
	m_attackRangeSqrd = m_attackRange * m_attackRange;
}

void EnemyComponenet::OnUpdate(Engine::Timestep ts) {
	
	glm::vec2 offset = { m_player->GetComponent<Engine::TransformComponent>().position.x - m_Entity->GetComponent<Engine::TransformComponent>().position.x, m_player->GetComponent<Engine::TransformComponent>().position.y - m_Entity->GetComponent<Engine::TransformComponent>().position.y  };
	float lenSqrd = glm::dot(offset, offset);	// Length squared

	//Attack
	m_attackTimer -= ts.GetMilliseconds();
	if (m_attackTimer <= 0) {
		if (lenSqrd < m_attackRangeSqrd && offset != glm::vec2(0)) {
			Attack(glm::normalize(offset));
		}

		m_attackTimer = m_attackSpeed;
	}

	// Movement
	// If idle, wait until timer runs out before moving. Enemy is idle when it stops moving and timer starts then
	// Right now, the timer still goes down even if the enemy should still be idle, but it doesn't really matter
	glm::vec2 dir = glm::vec2(0);
	m_idleTimer -= ts.GetMilliseconds();
	if (m_idleTimer <= 0) {

		// Only one at a time, can't have both axes set to 0 in this way
		if (abs(offset.x) < 48) {
			dir.x = 0;
			dir.y = (offset.y > 0) - (offset.y < 0);
		}
		else {
			dir.x = (offset.x > 0) - (offset.x < 0);

			if (abs(offset.y) < 48) dir.y = 0;
			else dir.y = (offset.y > 0) - (offset.y < 0);
		}

		dir = offset == glm::vec2(0) ? offset : glm::normalize(offset);

		if (lenSqrd > m_attackRangeSqrd) {
		}
		else if (lenSqrd < m_attackRangeSqrd / 2) {
			dir = -dir;
		}
		else {
			dir = glm::vec2(0);
			m_idleTimer = 700;

			// Chance to set a random direction for this idle period
			if (rand() % 5 == 0) {
				m_randDir = glm::vec2((rand() % 3) - 1, (rand() % 3) - 1);

				if (m_randDir != glm::vec2(0)) {
					m_randDir = glm::normalize(m_randDir);
				}
			}
			else {
				m_randDir = glm::vec2(0);
			}
		}
	}
	else {
		dir = m_randDir;
	}

	CharacterComponent::Move(dir, m_acceleration, m_maxSpeed, ts);

	CharacterComponent::OnUpdate(ts);
}

void EnemyComponenet::Attack(glm::vec2 dir) {

}