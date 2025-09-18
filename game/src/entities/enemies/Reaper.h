#pragma once
#include <engine.h>

#include "../Enemy.h"
#include "../../graphics/Anim.h"

struct ReaperComponent : public EnemyComponenet
{
	ReaperComponent() = default;
	ReaperComponent(const ReaperComponent&) = default;
	ReaperComponent(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player, std::unordered_map<std::string, Engine::Ref<Anim>>* animations);

	void Attack(glm::vec2 dir) override;
	virtual void OnUpdate(Engine::Timestep ts) override;
private:
	std::unordered_map<std::string, Animator*> m_Animations;
	float m_timer = 0.0f;
	bool m_isAttack = false;

	enum State {
		SPAWN,
		IDLE,
		ATK,
		DESPAWN,
	};

	State m_State = State::SPAWN;
};