#include "GameLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "entities/Player.h"


GameLayer::GameLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight()),
	m_CheckerboardUUID(0), m_Scene()
{
}

void GameLayer::OnAttach()
{
	m_WFC = new WaveFunctionCollapse("assets/WFC/kenny.json", &m_Scene, { 25, 25 }, { -Engine::Application::getApplication()->getWindow()->GetWidth()/4, -Engine::Application::getApplication()->getWindow()->GetHeight()/4, 0 }, { 2, 2 });

	//for (int i = 0; i < 100; i++)
	//{
	//	m_WFC->SetTile(i * 100, "psychicWallSouth");
	//	m_WFC->SetTile(i * 100 + 99, "psychicWallNorth");
	//	m_WFC->SetTile(i, "psychicWallEast");
	//	m_WFC->SetTile(100 * 99 + i, "psychicWallWest");
	//}
	//m_WFC->SetTile(24, "psychicWallNorthEast");
	//m_WFC->SetTile(624, "psychicWallNorthWest");

	//m_WFC->SetTile(26, "psychicWallNorthEast");

	m_WorldGenThread = std::thread(&WaveFunctionCollapse::ColapseLoop, m_WFC);
	if (m_WorldGenThread.joinable())
		m_WorldGenThread.detach();

	Engine::Ref<Engine::Texture2D> arrowTexture = Engine::Texture2D::Create("assets/textures/arrow.png");
	Engine::Ref<Engine::Texture2D> checkboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");

	Engine::Entity Checkboard = m_Scene.AddEntity("Checkboard");
	Checkboard.GetComponent<Engine::TransformComponent>().position = { 0.f, 200.f, 0.1f };
	Checkboard.GetComponent<Engine::TransformComponent>().scale = { 32.f * 8, 32.f * 8 };
	Checkboard.GetComponent<Engine::TransformComponent>().rotation = 35.f;
	Checkboard.AddComponent<Engine::SpriteRendererComponent>();
	Checkboard.GetComponent<Engine::SpriteRendererComponent>().texture = checkboardTexture;
	//Checkboard.GetComponent<Engine::MetaDataComponent>().hide = true;

	Engine::Entity Ground = m_Scene.AddEntity("Ground");
	Ground.GetComponent<Engine::TransformComponent>().position = { -32.f * 2, -100.f, 0.1f };
	Ground.GetComponent<Engine::TransformComponent>().scale = { 32.f * 16, 32.f };
	Ground.GetComponent<Engine::TransformComponent>().rotation = 15.f;
	Ground.AddComponent<Engine::SpriteRendererComponent>();
	Ground.GetComponent<Engine::SpriteRendererComponent>().colour = { 1, 1, 1, 1 };
	Ground.AddComponent<Engine::RigidBody2DComponent>();
	Ground.AddComponent<Engine::BoxCollider2DComponent>();

	m_Player = m_Scene.AddEntity("Player");
	m_Player.GetComponent<Engine::TransformComponent>().position = { 0.f, 0.f, 0.9f };
	m_Player.AddComponent<Engine::NativeScriptComponent>().Bind<PlayerController>();	

	m_Camera = m_Scene.AddEntity("Camera");
	m_Camera.AddComponent<Engine::OrthographicCameraComponent>().camera = &m_CameraController.GetCamera();
	m_Scene.SetPrimaryCamera(m_Camera);

	Checkboard.GetComponent<Engine::MetaDataComponent>().parent = m_Player.getUUID();

	m_CameraController.SetZoomLevel(128);
}

void GameLayer::OnDetach()
{
	if (m_WorldGenThread.joinable())
		m_WorldGenThread.join();
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	m_Scene.UpdateScene(ts);

	m_CameraController.setPosition(-m_Player.GetComponent<Engine::TransformComponent>().position);
	m_CameraController.OnUpdate(ts);
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	m_Scene.RenderScene();
	m_WFC->Render(&m_CameraController);
}

void GameLayer::OnImGuiRender()
{
	EG_PROFILE_FUNCTION();
	m_WFC->OnImGuiRender();
	return;
	// Begin with window. Requires window name
	ImGui::Begin("Window info");

	ImGui::SeparatorText("Window Size");
	ImGui::Text((std::string("Width: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetWidth())).c_str());
	ImGui::Text((std::string("Height: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetHeight())).c_str());

	ImGui::Text(glm::to_string(m_Player.GetComponent<Engine::VelocityComponent>().velocity).c_str());
	ImGui::End();
}

void GameLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(GameLayer::SprintKey));
}

bool GameLayer::SprintKey(Engine::KeyPressedEvent& e)
{
	/*if (e.GetKeyCode() == EG_KEY_LEFT_SHIFT && e.GetRepeatCount() == 0 && m_Player.GetComponent<PlayerComponent>().dashIndex == 0) {
		m_Player.GetComponent<PlayerComponent>().dashIndex = .2f;
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_SPACE && e.GetRepeatCount() == 0 && m_Player.GetComponent<PlayerComponent>().dashIndex == 0) {
		m_Player.GetComponent<PlayerComponent>().Attack();
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_F8 && e.GetRepeatCount() == 0) {
		m_Scene.RemoveEntity(m_Player);
		m_Player = m_Scene.AddEntity("Player");
		m_Player.GetComponent<Engine::TransformComponent>().position = { 0.f, 0.f, 0.9f };
		m_Player.AddComponent<PlayerComponent>(&m_Scene, &m_Player, &m_Animations);
	}*/
#if !defined(EG_DIST)
	if (e.GetKeyCode() == EG_KEY_PAGE_UP && e.GetRepeatCount() == 0) {
		m_CameraController.SetZoomLevel(m_CameraController.GetZoomLevel() + 128);
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_PAGE_DOWN && e.GetRepeatCount() == 0) {
		m_CameraController.SetZoomLevel(m_CameraController.GetZoomLevel() - 128);
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_HOME && e.GetRepeatCount() == 0) {
		m_CameraController.SetZoomLevel(128);
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_F6 && e.GetRepeatCount() == 0 && !m_WFC->generating) {
		if (m_WorldGenThread.joinable())
			m_WorldGenThread.join();
		m_WFC->~WaveFunctionCollapse();

		m_WFC = new WaveFunctionCollapse("assets/WFC/kenny.json", &m_Scene, { 25, 25 }, { -Engine::Application::getApplication()->getWindow()->GetWidth() / 4, -Engine::Application::getApplication()->getWindow()->GetHeight() / 4, 0 }, { 2, 2 });
		m_WorldGenThread = std::thread(&WaveFunctionCollapse::ColapseLoop, m_WFC);
		if (m_WorldGenThread.joinable())
			m_WorldGenThread.detach();
		return true;
	}
#endif
	// WFC Debug Window
	if (e.GetKeyCode() == EG_KEY_F5 && e.GetRepeatCount() == 0) {
		m_WFC->showImGuiWindow = !m_WFC->showImGuiWindow;
		return true;
	}
	return false;
}

glm::vec2 GameLayer::GetMouseGamePosition() {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Input::GetMouseX(), float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Input::GetMouseY(), 1.0f),
		glm::mat4(1.0f),
		m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}