#pragma once

#include <engine.h>
// idk why it needs ../
#include "../graphics/Anim.h"

#include "Character.h"

struct PlayerComponent : public CharacterComponent
{
	float dashIndex = 0;

	PlayerComponent() = default;
	PlayerComponent(const PlayerComponent&) = default;
	PlayerComponent(Engine::Scene* scene, Engine::Entity* entity, const std::unordered_map<std::string, Engine::Ref<Anim>>* animations);

	virtual void OnUpdate(Engine::Timestep ts) override;
	void Attack();
private:
	float m_acceleration;
	float m_deceleration;
	float m_maxSpeed;

	float m_SleepTimer = 0;
	std::unordered_map<std::string, Animator*> m_Animations;

	Engine::Entity m_ZZZ[3];
	glm::vec2 m_DirCopy = glm::vec2(0);
};

