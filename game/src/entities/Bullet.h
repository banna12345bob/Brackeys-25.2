#pragma once

#include <engine.h>
#include "Player.h"

class Bullet : public Engine::Entity
{
public:
	Bullet(Engine::Scene* scene, std::string name, Player* player, int speed, float initalAngle = 0.f, float lifetime = 2.f);

	void OnUpdate(Engine::Timestep ts) override;

	float theta;
	float speed;
	float lifetime;
private:
	bool OverLappingWithEntity(Engine::Entity* entity);

private:
	Player* m_Player;

};

