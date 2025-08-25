#pragma once

#include <engine.h>

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
private:
	bool SprintKey(Engine::KeyPressedEvent& e);
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Scene* m_Scene;

	float m_CurrentFrame = 0;

	float m_PlayerDeceleration = 5.f;
	float m_PlayerAcceleration = 2.f;
	float m_PlayerMaxAcceleration = 3.f;

	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;
};

