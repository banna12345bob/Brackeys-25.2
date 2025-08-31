#pragma once

#include <engine.h>

class Bullet : public Engine::Entity
{
public:
	Bullet(Engine::Scene& scene, std::string name, Engine::UUID playerUUID, int speed, float initalAngle = 0.f);

	void OnUpdate(Engine::Timestep ts) override;

	float theta;
	float speed;
	float lifetime;
private:
	bool OverLappingWithEntity(Engine::Entity* entity);

private:
	Engine::UUID m_PlayerUUID;

};

