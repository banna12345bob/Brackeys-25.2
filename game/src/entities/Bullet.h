#pragma once

#include <engine.h>

struct BulletComponenet
{
	float theta;
	float speed;
	float lifetime;

	BulletComponenet() = default;
	BulletComponenet(const BulletComponenet&) = default;
	BulletComponenet(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player, int speed, float initalAngle = 0.f, float lifetime = -1.f);
	void OnUpdate(Engine::Timestep ts);
private:
	bool OverLappingWithEntity(Engine::Entity* entity);
	Engine::Entity* m_Entity;
	Engine::Scene* m_Scene;

	Engine::Entity* m_Player;
};

