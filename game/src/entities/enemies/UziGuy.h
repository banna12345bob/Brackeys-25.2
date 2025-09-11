#pragma once
#include "../Enemy.h"

class UziGuy : public Enemy
{
public:
	UziGuy(std::string name, Engine::Scene* scene, Player* player);
	void Attack(glm::vec2 dir) override;
};