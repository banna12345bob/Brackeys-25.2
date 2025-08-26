#pragma once

#include "engine.h"

class Player : public Engine::Entity
{
public:
	Player();
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;

private:
	int m_health;
	float m_acceleration;
	float m_maxSpeed;

	
};

