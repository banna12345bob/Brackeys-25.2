#pragma once
#include "../Enemy.h"
#include <engine.h>
#include "../../graphics/Anim.h"
#include "../Character.h"

class Reaper : public Enemy {
public:
	Reaper(std::string name, Engine::Scene* scene, Player* player, std::unordered_map<std::string, Engine::Ref<Anim>>* animations);
	void Attack(glm::vec2 dir) override;
	void OnUpdate(Engine::Timestep ts) override;
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