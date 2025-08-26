#include "Player.h"

Player::Player() : Engine::Entity("Player"),
	m_acceleration(3000),
	m_maxSpeed(256),
	m_health(10) 
{

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
	if ((newDir.x != 0 && GetAcceleration()->acceleration.x == 0) || newDir.x * GetAcceleration()->acceleration.x > 0) {	// Same direction or just started moving
		if (abs(GetAcceleration()->acceleration.x) < m_maxSpeed) {
			GetAcceleration()->acceleration.x += m_acceleration * newDir.x * ts;
		}
	}
	else if (newDir.x == 0) {																				// Not moving on this axis
		if (GetAcceleration()->acceleration.x != 0) {														// Decelerate
			int sign = (GetAcceleration()->acceleration.x > 0) - (GetAcceleration()->acceleration.x < 0);	// Hack I found on Stack Overflow
			GetAcceleration()->acceleration.x += m_acceleration * ts * -sign;										// Decelerate twice as fast
			if (GetAcceleration()->acceleration.x * sign < 0) {												// If we have gone past 0, set it to 0
				GetAcceleration()->acceleration.x = 0;
			}
		}
	}
	else {																									// Going in opposite direction
		GetAcceleration()->acceleration.x = m_acceleration * newDir.x * ts;
	}

	// Y axis
	if ((newDir.y != 0 && GetAcceleration()->acceleration.y == 0) || newDir.y * GetAcceleration()->acceleration.y > 0) {
		if (abs(GetAcceleration()->acceleration.y) < m_maxSpeed) {
			GetAcceleration()->acceleration.y += m_acceleration * newDir.y * ts;
		}
	}
	else if (newDir.y == 0) {
		if (GetAcceleration()->acceleration.y != 0) {
			int sign = (GetAcceleration()->acceleration.y > 0) - (GetAcceleration()->acceleration.y < 0);
			float accel = GetAcceleration()->acceleration.y;
			GetAcceleration()->acceleration.y += m_acceleration * ts * -sign;
			if (GetAcceleration()->acceleration.y * sign < 0) {
				GetAcceleration()->acceleration.y = 0;
			}
		}
	}
	else {
		GetAcceleration()->acceleration.y = m_acceleration * newDir.y * ts;
	}
	

	Engine::Entity::OnUpdate(ts);
}

void Player::OnRender() {
	Engine::Entity::OnRender();
}