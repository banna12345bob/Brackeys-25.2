#pragma once

#include <engine.h>
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "environment/WFC.h"

class GameLayer : public Engine::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;
	glm::vec2 GetMouseGamePosition();
private:
	bool SprintKey(Engine::KeyPressedEvent& e);
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Scene* m_Scene;

	float m_CurrentFrame = 0;

	Player* m_Player;

	WaveFunctionCollapse m_WFC;

	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;
};

