#include "Player.h"

Player::Player(Engine::Scene& scene, std::unordered_map<std::string, Engine::Ref<Anim>>* animations) 
	: Engine::Entity("Player", scene),
	m_deceleration(20.f),
	m_maxSpeed(250.f),
	m_acceleration(3000.f),
	health(10),
	m_dashing(false)
{
	GetTransform()->scale = { 32.f, 32.f };
	m_BoundingBox = Engine::BoundingBox(-GetTransform()->scale.x / 2, -GetTransform()->scale.y / 2, 31, 30);

	m_Animations["player_idle"] = new Animator(animations->at("player_idle"));
	m_Animations["player_back"] = new Animator(animations->at("player_back"));
	m_Animations["player_fwd"] = new Animator(animations->at("player_fwd"));
	m_Animations["player_left"] = new Animator(animations->at("player_left"));
	m_Animations["player_dead"] = new Animator(animations->at("player_dead"));
	m_Animations["player_sleep"] = new Animator(animations->at("player_sleep"));

	m_ZZZ = new Engine::Entity("zzz", scene);
	Engine::Ref<Engine::Texture2D> zzz = Engine::Texture2D::Create("assets/textures/zzz.png");
	m_ZZZ->GetTransform()->position = { 0, 0, 0.5 };
	m_ZZZ->GetSpriteRenderer()->texture = zzz;
	m_ZZZ->GetTransform()->scale = {16, 16 };
	scene.AddEntity(m_ZZZ);
	m_ZZZ->hide = true;
}

void Player::OnUpdate(Engine::Timestep ts) {

	// Movement
	for (auto& animation : m_Animations)
	{
		animation.second->progress += ts.GetSeconds();
	}

	// Currently confined to what the camera sees
	glm::vec2 dir = glm::vec2(0.f);
	if (Engine::Input::IsKeyPressed(EG_KEY_W)) {
		dir.y += 1;
		GetSpriteRenderer()->texture = m_Animations["player_back"]->Get();
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_A)) {
		dir.x -= 1;
		GetSpriteRenderer()->texture = m_Animations["player_left"]->Get();
		GetSpriteRenderer()->texture->flipAcrossYAxis(false);
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_S)) {
		dir.y -= 1;
		GetSpriteRenderer()->texture = m_Animations["player_fwd"]->Get();
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_D)) {
		dir.x += 1;
		GetSpriteRenderer()->texture = m_Animations["player_left"]->Get();
		GetSpriteRenderer()->texture->flipAcrossYAxis(true);
	}

	if (glm::length(dir) == 0) {
		GetSpriteRenderer()->texture = m_Animations["player_idle"]->Get();
		m_SleepTimer += ts.GetSeconds();
	} else
		m_SleepTimer = 0;

	if (m_SleepTimer > 10)
	{
		GetSpriteRenderer()->texture = m_Animations["player_sleep"]->Get();
		m_ZZZ->hide = false;
		m_ZZZ->GetVelocity()->velocity = { 0.5f, 1.f, 0 };
		m_ZZZ->GetVelocity()->scaleVelocity = { -1, -1 };
		m_ZZZ->GetSpriteRenderer()->colour.a -= 0.05 * ts;
		if (m_ZZZ->GetSpriteRenderer()->colour.a < 0.25) {
			m_ZZZ->hide = true;
		}
		if (m_ZZZ->GetSpriteRenderer()->colour.a < 0) {
			m_ZZZ->hide = false;
			m_ZZZ->GetSpriteRenderer()->colour.a = 1;
			m_ZZZ->GetTransform()->scale = { 16, 16 };
			m_ZZZ->GetTransform()->position = GetTransform()->position + glm::vec3(0, 4, 0.5);
		}
	} else {
		m_ZZZ->GetTransform()->scale = { 16, 16 };
		m_ZZZ->GetSpriteRenderer()->colour.a = 1;
		m_ZZZ->GetTransform()->position = GetTransform()->position + glm::vec3(0, 4, 0.5);
		m_ZZZ->hide = true;
	}

	if (health < 1)
	{
		GetSpriteRenderer()->texture = m_Animations["player_dead"]->Get();
		dir = glm::vec2(0.f);
	}

	Engine::Entity::Move(dir, m_acceleration, m_maxSpeed, ts);

	Engine::Entity::OnUpdate(ts);
}

void Player::OnRender() {
	Engine::Entity::OnRender();
}