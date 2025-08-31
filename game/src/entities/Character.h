#pragma once

#include <engine.h>

class Character : public Engine::Entity
{
public:
	Character(std::string name, Engine::Scene& scene);
	void OnUpdate(Engine::Timestep ts) override;

	void Damage(int damage);

	int health;
protected:
	float m_HurtIndex = 0;
};

