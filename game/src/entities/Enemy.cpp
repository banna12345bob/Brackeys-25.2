#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(Player* player)
	: Engine::Entity("Enemy"),
	m_health(10),
	m_acceleration(700),
	m_maxSpeed(80),
	m_attackRange(128),
	m_player(player)
{
	GetTransform()->scale = { 16.f, 16.f };
	m_attackRangeSqrd = m_attackRange * m_attackRange;
}

void Enemy::OnUpdate(Engine::Timestep ts) {
	glm::vec2 offset = { GetTransform()->position.x - m_player->GetTransform()->position.x, GetTransform()->position.y - m_player->GetTransform()->position.y};
	float lenSqrd = glm::dot(offset, offset);	// Length squared

	glm::vec2 dir = { 0,0 };

	if (abs(offset.x) < 1.0f) dir.x = 0;
	else dir.x = (offset.x > 0) - (offset.x < 0);

	if (abs(offset.y) < 1.0f) dir.y = 0;
	else dir.y = (offset.y > 0) - (offset.y < 0);

	if (lenSqrd > m_attackRangeSqrd) {
		dir = -dir;
	}

	Entity::Move(dir, m_acceleration, m_maxSpeed, ts);

	Entity::OnUpdate(ts);
}

void Enemy::OnRender() {
	Engine::Entity::OnRender();
}