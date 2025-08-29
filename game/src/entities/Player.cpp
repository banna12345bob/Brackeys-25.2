#include "Player.h"

Player::Player() : Engine::Entity("Player"),
	m_deceleration(20.f),
	m_maxSpeed(250.f),
	m_acceleration(3000.f),
	health(10)
{
	GetTransform()->scale = { 32.f, 32.f };
}

void Player::OnUpdate(Engine::Timestep ts) {

	// Movement

	// Currently confined to what the camera sees
	glm::vec2 direction = glm::vec2(0.f);
	bool hitWall = false;
	if (Engine::Input::IsKeyPressed(EG_KEY_D)) {
		direction.x = 1;
		//if (GetTransform()->position.x > 128) {
		//	direction.x = 0;
		//	hitWall = true;
		//}
	} else if (Engine::Input::IsKeyPressed(EG_KEY_A)) {
		direction.x = -1;
		//if (GetTransform()->position.x < -128) {
		//	direction.x = 0;
		//	hitWall = true;
		//}
	}
	if ((Engine::Input::IsKeyPressed(EG_KEY_D) && GetVelocity()->velocity.x < 0) || (Engine::Input::IsKeyPressed(EG_KEY_A) && GetVelocity()->velocity.x > 0)) {
		GetVelocity()->velocity.x = 0;
	}

	if (Engine::Input::IsKeyPressed(EG_KEY_S) && -128 < GetTransform()->position.y < 128) {
		direction.y = -1;
		//if (GetTransform()->position.y < -128) {
		//	direction.y = 0;
		//	hitWall = true;
		//}
	} else if (Engine::Input::IsKeyPressed(EG_KEY_W) && -128 < GetTransform()->position.y < 128) {
		direction.y = 1;
		//if (GetTransform()->position.y > 128) {
		//	direction.y = 0;
		//	hitWall = true;
		//}
	}
	//if ((Engine::Input::IsKeyPressed(EG_KEY_W) && GetVelocity()->velocity.y < 0) || (Engine::Input::IsKeyPressed(EG_KEY_S) && GetVelocity()->velocity.y > 0)) {
	//	GetVelocity()->velocity.y = 0;
	//}

	//if (glm::length(direction) != 0 && glm::length(GetVelocity()->velocity) < m_maxSpeed) {
	//	// v = v + direction.nor * a
	//	// Got to take the normal of the direction so that player speed is calculated correctly on diagonals
	//	GetVelocity()->velocity += glm::normalize(direction) * m_acceleration * ts.GetSeconds();
	//} else {
	//	// v = v + -v * deceleration
	//	GetVelocity()->velocity += -GetVelocity()->velocity * m_deceleration * ts.GetSeconds();
	//	if (glm::length(GetVelocity()->velocity) < 20.f)
	//		GetVelocity()->velocity = { 0.f, 0.f, 0.f };
	//}

	//// Stop all velocity when hit a wall
	//// Currently cannot wall slide
	//if (hitWall) {
	//	GetVelocity()->velocity = { 0.f, 0.f, 0.f };
	//}

	//if (glm::length(GetVelocity()->velocity) != 0)
	//	GetVelocity()->velocity = glm::normalize(GetVelocity()->velocity) * glm::length(GetVelocity()->velocity);

	Engine::Entity::Move(direction, m_acceleration, m_maxSpeed, ts);

	Engine::Entity::OnUpdate(ts);
}

void Player::OnRender() {
	Engine::Entity::OnRender();
}