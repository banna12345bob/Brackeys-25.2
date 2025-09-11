#pragma once
#include "../Enemy.h"

class PistolGuy : public Enemy
{
public:
	PistolGuy(std::string name, Engine::Scene* scene, Player* player);
	void Attack(glm::vec2 dir) override;
};