#pragma once

#include <engine.h>
#include "environment/WFC.h"
#include "graphics/Anim.h"

#include "SceneOverride.h"

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

	SceneOverride m_Scene;

	Engine::Entity m_Player;
	Engine::Entity m_Bullet;

	WaveFunctionCollapse* m_WFC;
	std::thread m_WorldGenThread;

	Engine::UUID m_CheckerboardUUID;
};

