#pragma once

#include <engine.h>

class Character : public Engine::Entity
{
public:
	Character(std::string name, Engine::Scene& scene, int health);
	void OnUpdate(Engine::Timestep ts) override;

	bool Damage(int damage);

	int health;

	bool invincible;
protected:
	float m_HurtIndex = 0;
};

