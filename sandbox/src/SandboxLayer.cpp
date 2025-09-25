#include "SandboxLayer.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>

#include <box2d/box2d.h>

SandboxLayer::SandboxLayer()
	: Layer("SandboxLayer"), m_CameraController(Engine::Application::getApplication()->getWindow()->GetWidth() / Engine::Application::getApplication()->getWindow()->GetHeight(), glm::vec3(0.f), true, true),
	m_Scene()
{
}

void SandboxLayer::OnAttach()
{
	//m_sandBoxTexture = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
	m_Tilesheet = Engine::Texture2D::Create("assets/textures/kenny/kenny_tiny_town.png");

	m_TreeTexture = Engine::SubTexture2D::CreateFromCoords(m_Tilesheet, { 4, 9 }, { 16, 16 }, { 1, 2 });
	m_MushroomsTexture = Engine::SubTexture2D::CreateFromCoords(m_Tilesheet, { 5, 8 }, { 16, 16 });

	m_Animation = Engine::Texture2D::Create("assets/textures/tile.png");

	m_Camera = m_Scene.AddEntity("Camera");
	m_Camera.AddComponent<Engine::OrthographicCameraComponent>().camera = &m_CameraController.GetCamera();
	m_Scene.SetPrimaryCamera(m_Camera);

	Engine::Entity Mouse = m_Scene.AddEntity("Mouse");
	Mouse.GetComponent<Engine::TransformComponent>().position = { 100, 100, 0.f };
	Mouse.AddComponent<Engine::SpriteRendererComponent>();
	Mouse.AddComponent<Engine::RigidBody2DComponent>().Type = Engine::RigidBody2DComponent::BodyType::Kinematic;
	Mouse.AddComponent<Engine::BoxCollider2DComponent>();

	Engine::Entity ArrowAnimation = m_Scene.AddEntity("Arrow");
	ArrowAnimation.GetComponent<Engine::TransformComponent>().position = { 0.f, 0.f, 0.5f };
	ArrowAnimation.AddComponent<Engine::SpriteRendererComponent>();
	ArrowAnimation.GetComponent<Engine::SpriteRendererComponent>().texture = m_Animation;
	ArrowAnimation.AddComponent<Engine::RigidBody2DComponent>();
	ArrowAnimation.AddComponent<Engine::BoxCollider2DComponent>();

	Engine::Entity Ground = m_Scene.AddEntity("Ground");
	Ground.GetComponent<Engine::TransformComponent>().position = { 0.f, -2.5f, 0.f };
	Ground.GetComponent<Engine::TransformComponent>().scale = { 10, 1 };
	Ground.AddComponent<Engine::SpriteRendererComponent>();
	Ground.AddComponent<Engine::RigidBody2DComponent>();
	Ground.AddComponent<Engine::BoxCollider2DComponent>();

	// Little test grid
	for (float x = -2.0f; x < 2.f; x += 0.25f)
	{
		for (float y = 2; y > -2.f; y -= 0.25f)
		{
			Engine::Entity box = m_Scene.AddEntity("Box"+std::to_string(x)+ std::to_string(y));
			box.GetComponent<Engine::TransformComponent>().position = { x, y, 0 };
			box.GetComponent<Engine::TransformComponent>().scale = { .1f, .1f };
			box.AddComponent<Engine::SpriteRendererComponent>();
			box.GetComponent<Engine::SpriteRendererComponent>().colour = {0, 1, 1, 1};
			box.AddComponent<Engine::RigidBody2DComponent>().Type = Engine::RigidBody2DComponent::BodyType::Dynamic;
			box.AddComponent<Engine::BoxCollider2DComponent>();
		}
	}

	m_CameraController.SetZoomLevel(2.f);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	m_Scene.UpdateScene(ts);

	if (Engine::Input::IsMouseButtonPressed(EG_MOUSECODE_LEFT))
	{
		b2Body_SetTransform(m_Scene.GetEntity("Mouse").GetComponent<Engine::RigidBody2DComponent>().Box2DBodyID, { GetMouseGamePosition().x, GetMouseGamePosition().y }, b2Rot_identity);
	} else {
		b2Body_SetTransform(m_Scene.GetEntity("Mouse").GetComponent<Engine::RigidBody2DComponent>().Box2DBodyID, { 100.f, 100.f }, b2Rot_identity);
	}
}

void SandboxLayer::OnRender()
{
	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 0 });
	Engine::RenderCommand::Clear();

	m_Scene.RenderScene();
}

void SandboxLayer::OnImGuiRender()
{
	EG_PROFILE_FUNCTION();

	if (!m_ShowImGuiWindow)
		return;

	// Begin with window. Requires window name
	ImGui::Begin("Window info");

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

glm::vec2 SandboxLayer::GetMouseGamePosition() {
	glm::vec2 posVec = glm::unProject(
		glm::vec3(Engine::Input::GetMouseX(), float(Engine::Application::getApplication()->getWindow()->GetHeight()) - Engine::Input::GetMouseY(), 1.0f),
		glm::mat4(1.0f),
		m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::vec4(0.0f, 0.0f, float(Engine::Application::getApplication()->getWindow()->GetWidth()), float(Engine::Application::getApplication()->getWindow()->GetHeight()))
	);

	return posVec;
}

void SandboxLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}