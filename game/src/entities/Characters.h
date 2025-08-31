#pragma once

#include <engine.h>

class Character : public Engine::Entity
{
	Player(Engine::Scene& scene, std::unordered_map<std::string, Engine::Ref<Anim>>* animations);

};

