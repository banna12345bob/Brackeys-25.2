#include "Player.h"

Player::Player(Engine::Scene& scene, std::unordered_map<std::string, Engine::Ref<Anim>>* animations) : Engine::Entity("Player", scene),
	m_deceleration(20.f),
	m_maxSpeed(250.f),
	m_acceleration(3000.f),
	health(10),
	m_dashing(false)
{
	GetTransform()->scale = { 32.f, 32.f };
	m_BoundingBox = Engine::BoundingBox(-16, -16, 32, 32);
	m_PlayerAnimator = new Animator(animations->at("player_idle"));
}

void Player::OnUpdate(Engine::Timestep ts) {

	// Movement
	m_PlayerAnimator->progress += ts.GetSeconds();
	GetSpriteRenderer()->texture = m_PlayerAnimator->Get();

	// Currently confined to what the camera sees
	glm::vec2 dir = glm::vec2(0.f);
	if (Engine::Input::IsKeyPressed(EG_KEY_W)) {
		dir.y += 1;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_A)) {
		dir.x -= 1;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_S)) {
		dir.y -= 1;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_D)) {
		dir.x += 1;
	}

	Engine::Entity::Move(dir, m_acceleration, m_maxSpeed, ts);

	Engine::Entity::OnUpdate(ts);
}

void Player::OnRender() {
	Engine::Entity::OnRender();
}