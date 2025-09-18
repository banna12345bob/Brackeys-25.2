#pragma once
#include "../Enemy.h"

struct UziGuyComponenet : public EnemyComponenet
{
	UziGuyComponenet() = default;
	UziGuyComponenet(const UziGuyComponenet&) = default;
	UziGuyComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player);

	void Attack(glm::vec2 dir) override;
};