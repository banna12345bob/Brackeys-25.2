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

	Engine::Ref<Engine::Texture2D> zzz = Engine::Texture2D::Create("assets/textures/zzz.png");

	for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity*); i++)
	{
		m_ZZZ[i] = new Engine::Entity("zzz"+std::to_string(i), scene);
		m_ZZZ[i]->GetSpriteRenderer()->texture = zzz;
		m_ZZZ[i]->GetTransform()->position = {0, 0, 0.1*i};
		m_ZZZ[i]->GetTransform()->scale = {16, 16};
		m_ZZZ[i]->hide = true;
		scene.AddEntity(m_ZZZ[i]);
	}
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
		for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity*) - 1; i++)
		{
			m_ZZZ[i]->hide = false;
			m_ZZZ[i]->GetVelocity()->velocity = { 0.5f, 1.f, 0 };
			m_ZZZ[i]->GetVelocity()->scaleVelocity = { -1, -1 };
			m_ZZZ[i]->GetVelocity()->rotationVelocity = -2.5f;
			m_ZZZ[i]->GetSpriteRenderer()->colour.a -= 0.05 * ts;
			if (m_ZZZ[i]->GetSpriteRenderer()->colour.a < 0.75)
			{
				m_ZZZ[i+1]->hide = false;
				m_ZZZ[i+1]->GetVelocity()->velocity = { 0.5f, 1.f, 0 };
				m_ZZZ[i+1]->GetVelocity()->scaleVelocity = { -1, -1 };
				m_ZZZ[i+1]->GetVelocity()->rotationVelocity = -2.5f;
				m_ZZZ[i+1]->GetSpriteRenderer()->colour.a -= 0.05 * ts;
			}
			if (m_ZZZ[i]->GetSpriteRenderer()->colour.a < 0.5) {
				m_ZZZ[i]->hide = true;
			}
			if (m_ZZZ[i+1]->GetSpriteRenderer()->colour.a < 0.5) {
				m_ZZZ[i+1]->hide = true;
			}

			if (m_ZZZ[i]->GetSpriteRenderer()->colour.a < 0.25) {
				m_ZZZ[i]->hide = false;
				m_ZZZ[i]->GetSpriteRenderer()->colour.a = 1;
				m_ZZZ[i]->GetTransform()->scale = { 16, 16 };
				m_ZZZ[i]->GetTransform()->rotation = 0;
				m_ZZZ[i]->GetTransform()->position = GetTransform()->position + glm::vec3(0, 4, 0.1*i);
			}
			if (m_ZZZ[i+1]->GetSpriteRenderer()->colour.a < 0.25) {
				m_ZZZ[i+1]->hide = false;
				m_ZZZ[i+1]->GetSpriteRenderer()->colour.a = 1;
				m_ZZZ[i+1]->GetTransform()->scale = { 16, 16 };
				m_ZZZ[i+1]->GetTransform()->rotation = 0;
				m_ZZZ[i + 1]->GetTransform()->position = GetTransform()->position + glm::vec3(0, 4, 0.1 * (i+1));
			}
		}
	} else {
		for (int i = 0; i < sizeof(m_ZZZ) / sizeof(Engine::Entity*); i++)
		{
			m_ZZZ[i]->GetTransform()->scale = { 16, 16 };
			m_ZZZ[i]->GetTransform()->rotation = 0;
			m_ZZZ[i]->GetSpriteRenderer()->colour.a = 1;
			m_ZZZ[i]->GetTransform()->position = GetTransform()->position + glm::vec3(0, 4, 0.1*i);
			m_ZZZ[i]->hide = true;
		}
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