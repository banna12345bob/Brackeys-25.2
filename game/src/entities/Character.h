#pragma once

#include <engine.h>


struct CharacterComponent {
	int health;
	bool invincible = false;

	CharacterComponent() = default;
	CharacterComponent(const CharacterComponent&) = default;
	CharacterComponent(Engine::Scene* scene, Engine::Entity* entity);

	virtual void OnUpdate(Engine::Timestep ts);
	bool Damage(int damage);

	void Move(glm::vec2 dir, int acceleration, int maxSpeed, Engine::Timestep ts);
protected:
	float m_HurtIndex = 0;
	Engine::Entity* m_Entity;
	Engine::Scene* m_Scene;
};

