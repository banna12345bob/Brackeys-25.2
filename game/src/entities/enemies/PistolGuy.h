#pragma once
#include "../Enemy.h"

struct PistolGuyComponent : public EnemyComponenet
{
	PistolGuyComponent() = default;
	PistolGuyComponent(const PistolGuyComponent&) = default;
	PistolGuyComponent(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player);

	void Attack(glm::vec2 dir) override;
};