#pragma once

#include <engine.h>
// idk why it needs ../
#include "../graphics/Anim.h"

class Player : public Engine::Entity
{
public:
	Player(Engine::Scene& scene, std::unordered_map<std::string, Engine::Ref<Anim>>* animations);
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;

public:
	int health;
private:
	float m_acceleration;
	float m_deceleration;
	float m_maxSpeed;

	bool m_dashing;
	float m_SleepTimer = 0;
	std::unordered_map<std::string, Animator*> m_Animations;
};

