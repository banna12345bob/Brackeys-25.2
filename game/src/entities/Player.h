#pragma once

#include "engine.h"

class Player : public Engine::Entity
{
public:
	Player(Engine::Scene& scene);
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;

public:
	int health;
private:
	float m_acceleration;
	float m_deceleration;
	float m_maxSpeed;

	bool m_dashing;
};

