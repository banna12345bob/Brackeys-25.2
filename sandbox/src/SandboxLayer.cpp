#include "SandboxLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight(), glm::vec3(0.f), true, true)
{
}

void SandboxLayer::OnAttach()
{
	m_Scene = new Engine::Scene();


	//m_sandBoxTexture = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
	m_Tilesheet = Engine::Texture2D::Create("assets/textures/kenny/kenny_tiny_town.png");

	m_TreeTexture = Engine::SubTexture2D::CreateFromCoords(m_Tilesheet, { 4, 9 }, { 16, 16 }, { 1, 2 });
	m_MushroomsTexture = Engine::SubTexture2D::CreateFromCoords(m_Tilesheet, { 5, 8 }, { 16, 16 });

	m_Animation[0] = Engine::Texture2D::Create("assets/textures/tile.png");
	m_Animation[1] = Engine::Texture2D::Create("assets/textures/tile2.png");
	m_Animation[2] = Engine::Texture2D::Create("assets/textures/tile3.png");

	Engine::Entity* ArrowAnimation = new Engine::Entity("Arrow");
	ArrowAnimation->GetTransform()->position = { 0.f, 0.f, 0.5f };
	m_Scene->AddEntity(ArrowAnimation);

	// Little test grid
	for (float x = -2.0f; x < 2.f; x += 0.15f)
	{
		for (float y = 2; y > -2.f; y -= 0.15f)
		{
			Engine::Entity* box = new Engine::Entity("Box"+std::to_string(x)+ std::to_string(y));
			box->GetTransform()->position = { x, y, 0 };
			box->GetTransform()->scale = { .1f, .1f };
			box->GetSpriteRenderer()->colour = { 0, 1, 1, 1 };

			m_Scene->AddEntity(box);
		}
	}

	m_CameraController.SetZoomLevel(2.f);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(Engine::Timestep ts)
{
	m_CurrentFrame += 0.0005 * ts.GetMilliseconds();
	m_CameraController.OnUpdate(ts);

	m_Scene->GetEntity("Arrow")->GetSpriteRenderer()->texture = m_Animation[(int)m_CurrentFrame % 3];
}

void SandboxLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	//m_Scene->RenderScene(&m_CameraController.GetCamera());
}

void SandboxLayer::OnImGuiRender()
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

	ImGui::SeparatorText("Vsync");
	std::string currentVsyncMode;
	switch (Engine::Application::getApplication()->getWindow()->GetVSync()) {
	case -1:
		currentVsyncMode = "Adaptave";
		break;
	case 0:
		currentVsyncMode = "Off";
		break;
	case 1:
		currentVsyncMode = "On";
		break;
	default:
		currentVsyncMode = "Error";
		break;
	}
	if (ImGui::Button(("Vsync Mode: " + currentVsyncMode).c_str())) {
		Engine::Application::getApplication()->getWindow()->SetVSync((Engine::Application::getApplication()->getWindow()->GetVSync() == 0));
	}

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

void SandboxLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}