#include "Player.h"

Player::Player(Engine::Scene& scene) : Engine::Entity("Player", scene),
	m_deceleration(20.f),
	m_maxSpeed(250.f),
	m_acceleration(3000.f),
	health(10),
	m_dashing(false)
{
	GetTransform()->scale = { 32.f, 32.f };
	m_BoundingBox = Engine::BoundingBox(-16, -16, 32, 32);
}

void Player::OnUpdate(Engine::Timestep ts) {

	// Movement

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