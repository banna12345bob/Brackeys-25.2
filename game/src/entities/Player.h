#pragma once

#include <engine.h>
// idk why it needs ../
#include "../graphics/Anim.h"

#include "Character.h"

class Player : public Character
{
public:
	Player(Engine::Scene& scene, std::unordered_map<std::string, Engine::Ref<Anim>>* animations);
	void OnUpdate(Engine::Timestep ts) override;

	void Attack();

public:
	float dashIndex = 0;
private:
	float m_acceleration;
	float m_deceleration;
	float m_maxSpeed;

	float m_SleepTimer = 0;
	std::unordered_map<std::string, Animator*> m_Animations;

	Engine::Entity* m_ZZZ[3];
	glm::vec2 m_DirCopy = glm::vec2(0);
};

