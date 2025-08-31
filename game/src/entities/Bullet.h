#pragma once

#include <engine.h>

class Bullet : public Engine::Entity
{
public:
	Bullet(Engine::Scene& scene, std::string name, Engine::UUID playerUUID);

	void OnUpdate(Engine::Timestep ts) override;
private:
	bool OverLappingWithEntity(Engine::Entity* entity);

private:
	Engine::UUID m_PlayerUUID;
};

