#include "GameLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "entities/Bullet.h"
#include "environment/Room.h"


GameLayer::GameLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight()),
	m_CheckerboardUUID(0)
{
}

void GameLayer::OnAttach()
{
	m_Scene = new Engine::Scene();
	m_WFC = new WaveFunctionCollapse("assets/WFC/kenny.json", m_Scene, { 25, 25 }, { -Engine::Application::getApplication()->getWindow()->GetWidth()/4, -Engine::Application::getApplication()->getWindow()->GetHeight()/4, 0 }, { 2, 2 });

	m_Animations = Anim::LoadAnims("assets/animations/anim.json");

	//for (int i = 0; i < 25; i++)
	//{
	//	m_WFC->SetTile(i * 25, "stoneWall");
	//	m_WFC->SetTile(i * 25 + 24, "stoneWall");
	//	m_WFC->SetTile(i, "stoneWall");
	//	m_WFC->SetTile(25*24 + i, "stoneWall");
	//}


	m_WorldGenThread = std::thread(&WaveFunctionCollapse::ColapseLoop, m_WFC);
	if (m_WorldGenThread.joinable())
		m_WorldGenThread.detach();

	Engine::Ref<Engine::Texture2D> arrowTexture = Engine::Texture2D::Create("assets/textures/arrow.png");
	Engine::Ref<Engine::Texture2D> checkboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");

	Engine::Entity* Checkboard = new Engine::Entity("Checkboard", *m_Scene);
	Checkboard->GetTransform()->position = { 0.f, 0.f, 0.1f };
	Checkboard->GetTransform()->scale = { 32.f*8, 32.f*8 };
	Checkboard->GetSpriteRenderer()->texture = checkboardTexture;
	m_Scene->AddEntity(Checkboard, &m_CheckerboardUUID);

	m_Player = new Player(*m_Scene, &m_Animations);
	m_Player->GetTransform()->position = { 0.f, 0.f, 0.9f };
	m_Scene->AddEntity(m_Player);

	Bullet* bullet = new Bullet(*m_Scene, "test", m_Player->EntityUUID, 3.1415926);
	bullet->speed = 0;
	bullet->GetTransform()->position = { 100.f, 100.f, 0.2f };
	m_Scene->AddEntity(bullet);

	
	for (int i = 0; i < 3; i++) {
		Engine::BoundingBox box = Engine::BoundingBox(i * 32, 32, 32, 32);
		//m_Scene->AddCollisionBox(box);

		Enemy* enemy = new Enemy("Enemy", *m_Scene, *m_Player);
		enemy->GetSpriteRenderer()->texture = arrowTexture;
		enemy->GetTransform()->position = { 0.f, 0.f, 0.2f };
		m_Scene->AddEntity(enemy);
	}

	m_CameraController.SetZoomLevel(128);
}

void GameLayer::OnDetach()
{
	if (m_WorldGenThread.joinable())
		m_WorldGenThread.join();
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	/*glm::vec2 pos = GetMouseGamePosition();
	EG_TRACE("Mouse pos: {0}, {1}", pos.x, pos.y);*/

	m_Scene->UpdateScene(ts);

	m_Scene->GetEntity(m_CheckerboardUUID)->hide = !m_WFC->generating;

	m_CameraController.setPosition(-m_Player->GetTransform()->position);
	m_CameraController.OnUpdate(ts);
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	m_Scene->RenderScene(&m_CameraController.GetCamera());
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

	ImGui::Text(glm::to_string(m_Player->GetVelocity()->velocity).c_str());
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
	if (e.GetKeyCode() == EG_KEY_LEFT_SHIFT && e.GetRepeatCount() == 0 && m_Player->dashIndex == 0) {
		m_Player->dashIndex = .6f;
		return true;
	}
	if (e.GetKeyCode() == EG_KEY_SPACE && e.GetRepeatCount() == 0 && m_Player->dashIndex == 0) {
		m_Player->Attack();
		return true;
	}
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

		m_WFC = new WaveFunctionCollapse("assets/WFC/kenny.json", m_Scene, { 25, 25 }, { -Engine::Application::getApplication()->getWindow()->GetWidth() / 4, -Engine::Application::getApplication()->getWindow()->GetHeight() / 4, 0 }, { 2, 2 });
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