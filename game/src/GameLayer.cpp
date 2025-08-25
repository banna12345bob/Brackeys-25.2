#include "GameLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

GameLayer::GameLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight())
{
}

void GameLayer::OnAttach()
{
	m_Scene = new Engine::Scene();
	
	Engine::Ref<Engine::Texture2D> arrowTexture = Engine::Texture2D::Create("assets/textures/arrow.png");

	Engine::Entity* Arrow = new Engine::Entity("Arrow");
	Arrow->GetTransform()->position = { 0.f, 0.f, -0.5f };
	Arrow->GetSpriteRenderer()->texture = arrowTexture;
	m_Scene->AddEntity(Arrow);

	Engine::Entity* PlayerEntity = new Engine::Entity("Player");
	PlayerEntity->GetSpriteRenderer()->texture = arrowTexture;
	m_Scene->AddEntity(PlayerEntity);

	m_CameraController.SetZoomLevel(5.f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	m_CurrentFrame += 0.0005 * ts.GetMilliseconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_D) && glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) <= m_PlayerMaxAcceleration) {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.x += m_PlayerAcceleration * ts;
		m_Scene->GetEntity("Player")->GetTransform()->rotation = 270.f;
	}
	else if (Engine::Input::IsKeyPressed(EG_KEY_A) && glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) <= m_PlayerMaxAcceleration) {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.x -= m_PlayerAcceleration * ts;
		m_Scene->GetEntity("Player")->GetTransform()->rotation = 90.f;
	}
	else {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.x += -m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.x * m_PlayerDeceleration * ts.GetSeconds();
		if (glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) < 1.f)
			m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.x = 0;
	}

	if (Engine::Input::IsKeyPressed(EG_KEY_S) && glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) <= m_PlayerMaxAcceleration) {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.y -= m_PlayerAcceleration * ts;
		m_Scene->GetEntity("Player")->GetTransform()->rotation = 180.f;
	}
	else if (Engine::Input::IsKeyPressed(EG_KEY_W) && glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) <= m_PlayerMaxAcceleration) {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.y += m_PlayerAcceleration * ts;
		m_Scene->GetEntity("Player")->GetTransform()->rotation = 0.f;
	}
	else {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.y += -m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.y * m_PlayerDeceleration * ts.GetSeconds();
		if (glm::length(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration) < 1.f)
				m_Scene->GetEntity("Player")->GetAcceleration()->acceleration.y = 0;
	}

	m_Scene->UpdateScene(ts);

	m_CameraController.setPosition(-m_Scene->GetEntity("Player")->GetTransform()->position);
	m_CameraController.OnUpdate(ts);
}

void GameLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	m_Scene->RenderScene(&m_CameraController.GetCamera());
}

void GameLayer::OnImGuiRender()
{
	EG_PROFILE_FUNCTION();
	if (!m_ShowImGuiWindow)
		return;

	// Begin with window. Requires window name
	ImGui::Begin("Window info");

	//ImGui::Text(("FPS: " + std::to_string(Engine::Application::getApplication()->m_frameRate)).c_str());
	ImGui::Text(("AnimationIndex: " + std::to_string(m_CurrentFrame)).c_str());

	ImGui::SeparatorText("Window Size");
	ImGui::Text((std::string("Width: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetWidth())).c_str());
	ImGui::Text((std::string("Height: ") + std::to_string(Engine::Application::getApplication()->getWindow()->GetHeight())).c_str());

	ImGui::Text(glm::to_string(m_Scene->GetEntity("Player")->GetAcceleration()->acceleration).c_str());

	// Little header/tree demo
	if (ImGui::CollapsingHeader("Demo window")) {
		if (ImGui::TreeNode("Test"))
		{
			std::string demoWindowValue = m_ShowImGuiDemoWindow ? "True" : "False";
			if (ImGui::Button(("Show demo window: " + demoWindowValue).c_str())) {
				m_ShowImGuiDemoWindow = !m_ShowImGuiDemoWindow;
			}

			ImGui::TreePop();
		}
	}

	// Remember to end the window
	ImGui::End();

	if (m_ShowImGuiDemoWindow)
		ImGui::ShowDemoWindow(&m_ShowImGuiDemoWindow);
}

void GameLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(GameLayer::SprintKey));
}

bool GameLayer::SprintKey(Engine::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == EG_KEY_LEFT_SHIFT && e.GetRepeatCount() == 0) {
		m_Scene->GetEntity("Player")->GetAcceleration()->acceleration *= 15.f;
		return true;
	}
	return false;
}
