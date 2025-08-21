#pragma once

#include <engine.h>
#include <engine/core/EntryPoint.h>

#include "SandboxLayer.h"

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::Window::WindowProps props)
		: Engine::Application(props)
	{
		PushLayer(new SandboxLayer());
	}

	~sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::Window::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
