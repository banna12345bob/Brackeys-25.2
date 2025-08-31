#pragma once
#include "Reaper.h"
#include "../Bullet.h"
#include "../Player.h"

Reaper::Reaper(std::string name, Engine::Scene& scene, Player& player, std::unordered_map<std::string, Engine::Ref<Anim>>* animations)
	: Enemy(name, scene, player)
{
	health = 10;
	m_acceleration = 400;
	m_maxSpeed = 60;
	m_attackRange = 192;
	m_attackSpeed = 1200;

	m_Animations["reaper_atk"] = new Animator(animations->at("reaper_atk"));
	m_Animations["reaper_spawn"] = new Animator(animations->at("reaper_spawn"));
	m_Animations["reaper_despawn"] = new Animator(animations->at("reaper_despawn"));

	GetTransform()->scale = { 96 * 2, 80 * 2 };
}

void Reaper::OnUpdate(Engine::Timestep ts) {
	switch (m_State) {
	case State::SPAWN: 
		for (auto& animation : m_Animations)
		{
			animation.second->progress += ts.GetSeconds();
		}
		GetSpriteRenderer()->texture = m_Animations["reaper_spawn"]->Get();

		m_timer += ts.GetSeconds();
		if (m_timer >= 0.7f) {
			m_timer = 0.0f;
			m_State = State::IDLE;
		}
		break;
	case State::IDLE:
		GetSpriteRenderer()->texture = m_Animations["reaper_spawn"]->Get();
		break;
	case State::ATK:
		for (auto& animation : m_Animations)
		{
			animation.second->progress += ts.GetSeconds();
		}
		GetSpriteRenderer()->texture = m_Animations["reaper_atk"]->Get();

		m_timer += ts.GetSeconds();
		if (m_timer >= 0.5f) {
			m_timer = 0.0f;
			m_State = State::IDLE;
		}
		break;
	case State::DESPAWN:
		break;
	}

	Enemy::OnUpdate(ts);
}

void Reaper::Attack(glm::vec2 dir)
{
	Bullet* bullet = new Bullet(m_Scene, "test", m_player.EntityUUID, 150, glm::atan(dir.x, dir.y));
	bullet->GetTransform()->position = GetTransform()->position;
	m_Scene.AddEntity(bullet);
	
	m_State = State::ATK;
}