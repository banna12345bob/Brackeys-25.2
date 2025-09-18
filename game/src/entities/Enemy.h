#pragma once

#include <engine.h>

#include "Character.h"

struct EnemyComponenet : public CharacterComponent
{
	EnemyComponenet() = default;
	EnemyComponenet(const EnemyComponenet&) = default;
	EnemyComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player);

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void Attack(glm::vec2 dir);

protected:
	int m_acceleration;
	int m_maxSpeed;

	int m_attackRange;
	int m_attackRangeSqrd;
	int m_attackSpeed;
	int m_attackTimer;

	int m_idleTimer;
	glm::vec2 m_randDir;

	Engine::Entity* m_player;
};

