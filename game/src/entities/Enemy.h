#pragma once

#include <engine.h>

class Player;

class Enemy : public Engine::Entity
{
public:
	Enemy(Player* player);
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;

private:
	int m_health;
	int m_acceleration;
	int m_maxSpeed;
	int m_attackRange;
	int m_attackRangeSqrd;

	Player* m_player;
};

