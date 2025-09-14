#pragma once
#include "Reaper.h"
#include "../Bullet.h"

ReaperComponent::ReaperComponent(Engine::Scene* scene, Engine::Entity* entity, Engine::Entity* player, std::unordered_map<std::string, Engine::Ref<Anim>>* animations)
	: EnemyComponenet(scene, entity, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 1200;

	m_Animations["reaper_atk"] = new Animator(animations->at("reaper_atk"));
	m_Animations["reaper_spawn"] = new Animator(animations->at("reaper_spawn"));
	m_Animations["reaper_despawn"] = new Animator(animations->at("reaper_despawn"));

	m_Entity->GetComponent<Engine::TransformComponent>().scale = { 96 * 2, 80 * 2 };
}

void ReaperComponent::OnUpdate(Engine::Timestep ts) {
	switch (m_State) {
	case State::SPAWN: 
		for (auto& animation : m_Animations)
		{
			animation.second->progress += ts.GetSeconds();
		}
		m_Entity->GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["reaper_spawn"]->Get();

		m_timer += ts.GetSeconds();
		if (m_timer >= 0.7f) {
			m_timer = 0.0f;
			m_State = State::IDLE;
		}
		break;
	case State::IDLE:
		m_Entity->GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["reaper_spawn"]->Get();
		break;
	case State::ATK:
		for (auto& animation : m_Animations)
		{
			animation.second->progress += ts.GetSeconds();
		}
		m_Entity->GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["reaper_atk"]->Get();

		m_timer += ts.GetSeconds();
		if (m_timer >= 0.5f) {
			m_timer = 0.0f;
			m_State = State::IDLE;
		}
		break;
	case State::DESPAWN:
		break;
	}

	EnemyComponenet::OnUpdate(ts);
}

void ReaperComponent::Attack(glm::vec2 dir)
{
	Engine::Entity bullet = m_Scene->AddEntity("bullet");
	bullet.AddComponent<BulletComponenet>(m_Scene, &bullet, m_player, 150, glm::atan(dir.x, dir.y), 2);
	bullet.GetComponent<Engine::TransformComponent>().position = m_Entity->GetComponent<Engine::TransformComponent>().position;
	
	m_State = State::ATK;
}