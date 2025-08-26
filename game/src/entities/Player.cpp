#include "Player.h"

Player::Player() : Engine::Entity("Player"),
	m_acceleration(2000),
	m_maxSpeed(256),
	m_health(10) 
{
	GetTransform()->scale = { 32.f, 32.f };
}

void Player::OnUpdate(Engine::Timestep ts) {

	// Movement
	glm::vec2 newDir = { 0,0 };
	if (Engine::Input::IsKeyPressed(EG_KEY_W)) {
		newDir.y += 1;
		GetTransform()->rotation = 0.f;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_A)) {
		newDir.x -= 1;
		GetTransform()->rotation = 90.f;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_S)) {
		newDir.y -= 1;
		GetTransform()->rotation = 180.f;
	}
	if (Engine::Input::IsKeyPressed(EG_KEY_D)) {
		newDir.x += 1;
		GetTransform()->rotation = 270.f;
	}


	float maxSpeed = newDir.x != 0 && newDir.y != 0 ? sqrtf(2) : m_maxSpeed;							// If the player is moving diagonally, the max speed on either axis is root 2

	// X axis
	if ((newDir.x != 0 && GetVelocity()->velocity.x == 0) || newDir.x * GetVelocity()->velocity.x > 0) {	// Same direction or just started moving
		if (abs(GetVelocity()->velocity.x) < m_maxSpeed) {
			GetVelocity()->velocity.x += m_acceleration * newDir.x * ts;
		}
	}
	else if (newDir.x == 0) {																				// Not moving on this axis
		if (GetVelocity()->velocity.x != 0) {														// Decelerate
			int sign = (GetVelocity()->velocity.x > 0) - (GetVelocity()->velocity.x < 0);	// Hack I found on Stack Overflow
			GetVelocity()->velocity.x += m_acceleration * ts * -sign;										// Decelerate twice as fast
			if (GetVelocity()->velocity.x * sign < 0) {												// If we have gone past 0, set it to 0
				GetVelocity()->velocity.x = 0;
			}
		}
	}
	else {																									// Going in opposite direction
		GetVelocity()->velocity.x = m_acceleration * newDir.x * ts;
	}

	// Y axis
	if ((newDir.y != 0 && GetVelocity()->velocity.y == 0) || newDir.y * GetVelocity()->velocity.y > 0) {
		if (abs(GetVelocity()->velocity.y) < m_maxSpeed) {
			GetVelocity()->velocity.y += m_acceleration * newDir.y * ts;
		}
	}
	else if (newDir.y == 0) {
		if (GetVelocity()->velocity.y != 0) {
			int sign = (GetVelocity()->velocity.y > 0) - (GetVelocity()->velocity.y < 0);
			float accel = GetVelocity()->velocity.y;
			GetVelocity()->velocity.y += m_acceleration * ts * -sign;
			if (GetVelocity()->velocity.y * sign < 0) {
				GetVelocity()->velocity.y = 0;
			}
		}
	}
	else {
		GetVelocity()->velocity.y = m_acceleration * newDir.y * ts;
	}
	

	Engine::Entity::OnUpdate(ts);
}

void Player::OnRender() {
	Engine::Entity::OnRender();
}