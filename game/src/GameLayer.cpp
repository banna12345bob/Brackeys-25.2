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
	Engine::Ref<Engine::Texture2D> checkboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");

	Engine::Entity* Checkboard = new Engine::Entity("Checkboard");
	Checkboard->GetTransform()->position = { 0.f, 0.f, -0.5f };
	Checkboard->GetTransform()->scale = { 32.f*8, 32.f*8 };
	Checkboard->GetSpriteRenderer()->texture = checkboardTexture;
	m_Scene->AddEntity(Checkboard);

	m_player = new Player();
	m_player->GetSpriteRenderer()->texture = arrowTexture;
	m_Scene->AddEntity(m_player);

	Enemy* enemy = new Enemy(m_player);
	enemy->GetSpriteRenderer()->texture = arrowTexture;
	m_Scene->AddEntity(enemy);

	m_CameraController.SetZoomLevel(128);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	/*glm::vec2 pos = GetMouseGamePosition();
	EG_TRACE("Mouse pos: {0}, {1}", pos.x, pos.y);*/

	m_CurrentFrame += 0.0005 * ts.GetMilliseconds();

	m_Scene->UpdateScene(ts);

	//m_CameraController.setPosition(-m_Scene->GetEntity("Player")->GetTransform()->position);
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

	ImGui::Text(glm::to_string(m_Scene->GetEntity("Player")->GetVelocity()->velocity).c_str());

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
		m_Scene->GetEntity("Player")->GetVelocity()->velocity *= 15.f;
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