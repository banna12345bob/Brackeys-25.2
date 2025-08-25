#pragma once

#include <engine.h>
#include <engine/core/EntryPoint.h>

#include "GameLayer.h"

class game : public Engine::Application
{
public:

	game(Engine::Window::WindowProps props)
		: Engine::Application(props)
	{
		PushLayer(new GameLayer());
	}

	~game()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new game(Engine::Window::WindowProps("Game", 520, 520, "assets/textures/Oak_Log.png"));
}
