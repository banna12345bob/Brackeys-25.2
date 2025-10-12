#pragma once

#include <engine.h>
// idk why it needs ../
#include "../graphics/Anim.h"

class PlayerController : public Engine::ScriptableEntity
{
public:
	virtual void OnCreate() override
	{
		m_deceleration = 20.f;
		m_maxSpeed = 250.f;
		m_acceleration = 3000.f;
		health = 10;
		AddComponent<Engine::SpriteRendererComponent>();
		AddComponent<Engine::RigidBody2DComponent>().Type = Engine::RigidBody2DComponent::BodyType::Dynamic;
		GetComponent<Engine::RigidBody2DComponent>().FixedRotation = true;
		AddComponent<Engine::BoxCollider2DComponent>();

		std::unordered_map<std::string, Engine::Ref<Anim>> animations = Anim::LoadAnims("assets/animations/player.json");

		GetComponent<Engine::TransformComponent>().scale = { 32.f, 32.f };

		m_Animations["player_idle"] = new Animator(animations.at("player_idle"));
		m_Animations["player_back"] = new Animator(animations.at("player_back"));
		m_Animations["player_fwd"] = new Animator(animations.at("player_fwd"));
		m_Animations["player_left"] = new Animator(animations.at("player_left"));
		m_Animations["player_dead"] = new Animator(animations.at("player_dead"));
		m_Animations["player_sleep"] = new Animator(animations.at("player_sleep"));
		m_Animations["player_hurt"] = new Animator(animations.at("player_hurt"));
		m_Animations["player_dash_left"] = new Animator(animations.at("player_dash_left"));
		m_Animations["player_dash_up"] = new Animator(animations.at("player_dash_backwd"));
		m_Animations["player_dash_down"] = new Animator(animations.at("player_dash_fwd"));

		Engine::Ref<Engine::Texture2D> zzz = Engine::Texture2D::Create("assets/textures/zzz.png");

		for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity); i++)
		{
			m_ZZZ[i] = AddChild(GetScene()->AddEntity("zzz" + std::to_string(i)));
			m_ZZZ[i].AddComponent<Engine::SpriteRendererComponent>().texture = zzz;
			m_ZZZ[i].GetComponent<Engine::TransformComponent>().position = { 0, 0, 0.1 * i };
			m_ZZZ[i].GetComponent<Engine::TransformComponent>().scale = { 16, 16 };
			m_ZZZ[i].GetComponent<Engine::MetaDataComponent>().hide = true;
		}
	}

	virtual void OnUpdate(Engine::Timestep ts) override
	{
		for (auto& animation : m_Animations)
		{
			animation.second->progress += ts.GetSeconds();
		}

		glm::vec2 dir = glm::vec2(0.f);
		if (Engine::Input::IsKeyPressed(EG_KEY_W)) {
			dir.y += 1;
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_back"]->Get();
		}
		if (Engine::Input::IsKeyPressed(EG_KEY_A)) {
			dir.x -= 1;
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_left"]->Get();
			GetComponent<Engine::TransformComponent>().scale.x = abs(GetComponent<Engine::TransformComponent>().scale.x);
		}
		if (Engine::Input::IsKeyPressed(EG_KEY_S)) {
			dir.y -= 1;
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_fwd"]->Get();
		}
		if (Engine::Input::IsKeyPressed(EG_KEY_D)) {
			dir.x += 1;
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_left"]->Get();
			GetComponent<Engine::TransformComponent>().scale.x = -abs(GetComponent<Engine::TransformComponent>().scale.x);
		}
		if (dir != glm::vec2(0)) dir = glm::normalize(dir);

		if (glm::length(dir) == 0) {
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_idle"]->Get();
			m_SleepTimer += ts.GetSeconds();
		}
		else
			m_SleepTimer = 0;

		if (m_SleepTimer > 10 && health > 0)
		{
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_sleep"]->Get();
			for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity) - 1; i++)
			{
				m_ZZZ[i].GetComponent<Engine::MetaDataComponent>().hide = false;
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().position += glm::vec3{ 0.5f, 1.f, 0.f } * ts.GetSeconds();
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().scale += glm::vec2{ -1, -1 } * ts.GetSeconds();
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().rotation += -2.5f * ts.GetSeconds();
				m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a -= 0.05f * ts.GetSeconds();
				if (m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a < 0.75f)
				{
					m_ZZZ[i + 1].GetComponent<Engine::MetaDataComponent>().hide = false;
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().position += glm::vec3{ 0.5f, 1.f, 0.f } * ts.GetSeconds();
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().scale += glm::vec2{ -1, -1 } * ts.GetSeconds();
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().rotation += -2.5f * ts.GetSeconds();
					m_ZZZ[i + 1].GetComponent<Engine::SpriteRendererComponent>().colour.a -= 0.05f * ts;
				}
				if (m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a < 0.5f) {
					m_ZZZ[i].GetComponent<Engine::MetaDataComponent>().hide = true;
				}
				if (m_ZZZ[i + 1].GetComponent<Engine::SpriteRendererComponent>().colour.a < 0.5f) {
					m_ZZZ[i + 1].GetComponent<Engine::MetaDataComponent>().hide = true;
				}

				if (m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a < 0.25f) {
					m_ZZZ[i].GetComponent<Engine::MetaDataComponent>().hide = false;
					m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a = 1;
					m_ZZZ[i].GetComponent<Engine::TransformComponent>().scale = { 16, 16 };
					m_ZZZ[i].GetComponent<Engine::TransformComponent>().rotation = 0;
					m_ZZZ[i].GetComponent<Engine::TransformComponent>().position = glm::vec3(0, 4, 0.1 * i);
				}
				if (m_ZZZ[i + 1].GetComponent<Engine::SpriteRendererComponent>().colour.a < 0.25f) {
					m_ZZZ[i + 1].GetComponent<Engine::MetaDataComponent>().hide = false;
					m_ZZZ[i + 1].GetComponent<Engine::SpriteRendererComponent>().colour.a = 1;
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().scale = { 16, 16 };
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().rotation = 0;
					m_ZZZ[i + 1].GetComponent<Engine::TransformComponent>().position = glm::vec3(0, 4, 0.1 * (i + 1));
				}
			}
		}
		else {
			for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity); i++)
			{
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().scale = { 16, 16 };
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().rotation = 0;
				m_ZZZ[i].GetComponent<Engine::SpriteRendererComponent>().colour.a = 1;
				m_ZZZ[i].GetComponent<Engine::TransformComponent>().position = glm::vec3(0, 4, 0.1 * i);
				m_ZZZ[i].GetComponent<Engine::MetaDataComponent>().hide = true;
			}
		}

		if (m_HurtIndex > 0 && m_SleepTimer > 0)
		{
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_hurt"]->Get();
			m_SleepTimer = 0;
		}

		if (dashIndex > 0)
		{
			if (dashIndex > .05f) {
				m_DirCopy = dir;
				m_acceleration = 4500.f;
				m_maxSpeed = 1000;
			}
			else {
				m_acceleration = 125.f;
			}

			invincible = true;
			dir = m_DirCopy;
			if (dir.x > 0) {
				GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_dash_left"]->Get();
				GetComponent<Engine::TransformComponent>().scale.x = -abs(GetComponent<Engine::TransformComponent>().scale.x);
				dashIndex -= ts;
			}
			else if (dir.x < 0) {
				GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_dash_left"]->Get();
				GetComponent<Engine::TransformComponent>().scale.x = abs(GetComponent<Engine::TransformComponent>().scale.x);
				dashIndex -= ts;
			}
			else if (dir.y > 0) {
				GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_dash_up"]->Get();
				dashIndex -= ts;
			}
			else if (dir.y < 0) {
				GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_dash_down"]->Get();
				dashIndex -= ts;
			}
			else {
				dashIndex = 0;
			}

		}
		else {
			dashIndex = 0;
			m_DirCopy = glm::vec2(0);
			m_acceleration = 3000.f;
			m_maxSpeed = 250.f;
			invincible = false;
		}

		if (health < 1)
		{
			GetComponent<Engine::SpriteRendererComponent>().texture = m_Animations["player_dead"]->Get();
			invincible = true;
			dir = glm::vec2(0.f);
		}

		if (B2_IS_NON_NULL(GetComponent<Engine::RigidBody2DComponent>().Box2DBodyID))
		{
			b2Body_SetLinearVelocity(GetComponent<Engine::RigidBody2DComponent>().Box2DBodyID, { dir.x * m_acceleration, dir.y * m_acceleration });
		}
	}

private:
	float dashIndex = 0;

	float m_acceleration;
	float m_deceleration;
	float m_maxSpeed;
	std::unordered_map<std::string, Animator*> m_Animations;

	Engine::Entity m_ZZZ[3];
	float m_SleepTimer = 0;
	glm::vec2 m_DirCopy = glm::vec2(0);

	int health;
	bool invincible = false;
	float m_HurtIndex = 0;
};

