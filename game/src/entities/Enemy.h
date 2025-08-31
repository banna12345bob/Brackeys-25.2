#pragma once

#include <engine.h>

#include "Character.h"

class Player;

class Enemy : public Character
{
public:
	Enemy(std::string name, Engine::Scene& scene, Player& player);
	void OnUpdate(Engine::Timestep ts) override;
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

	Player& m_player;
};

