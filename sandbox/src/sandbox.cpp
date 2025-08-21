#pragma once

#include <engine.h>
#include <engine/core/EntryPoint.h>

#include "SandboxLayer.h"

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		getCallbackManager()->registerKeyboardCallback(keyboardEventCallback);
		getCallbackManager()->registerMouseDownCallback(mouseDownEventCallback);

		PushLayer(new SandboxLayer());
	}

	~sandbox()
	{
	}

	// A little example of how to do keyboard inputs
	static void keyboardEventCallback(void* callback) {
		EG_PROFILE_FUNCTION();
		if (Engine::Key::wasKeyPressed(EG_KEY_I))
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f);

		if (Engine::Key::wasKeyPressed(EG_KEY_O))
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/music/John Coltrane - Naima.wav", true, 0.8f);
	}

	static void mouseDownEventCallback(void* callback) {
		if (Engine::Mouse::isButtonDown(1))
			EG_TRACE("Left mouse button pressed");
		if (Engine::Mouse::isButtonDown(2))
			EG_TRACE("Middle mouse button pressed");
		if (Engine::Mouse::isButtonDown(3))
			EG_TRACE("Right mouse button pressed");
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
