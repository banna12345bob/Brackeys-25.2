#pragma once

#include <engine.h>

class SandboxLayer : public Engine::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Engine::Timestep ts) override;
	void OnRender() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_Tilesheet;
	Engine::Ref<Engine::Texture2D> m_sandBoxTexture;

	Engine::Ref<Engine::Texture2D> m_TreeTexture, m_MushroomsTexture;

	Engine::Ref<Engine::Texture2D> m_Animation[3];

	Engine::Scene* m_Scene;

	float m_CurrentFrame = 0;

	bool m_ShowImGuiWindow = true;
	bool m_ShowImGuiDemoWindow = false;
};

