#pragma once

#include <engine.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Frame {
	Engine::Ref<Engine::Texture2D> frameTexture;
	float duration;
};

class Anim {
public: 
	static std::unordered_map<std::string, Engine::Ref<Anim>> LoadAnims(std::string path);

	std::string name;
	std::vector<Frame> frames;
	Engine::Ref<Engine::Texture2D> texture;
	bool loop;
private:
	static json readJson(std::string path);

	Anim(std::string name, std::vector<Frame> frames, Engine::Ref<Engine::Texture2D> texture, bool loop);
};

class Animator {
public:
	Engine::Ref<Anim> anim;
	float progress = 0.0;

	Animator(Engine::Ref<Anim> anim);

	Engine::Ref<Engine::Texture2D> Get();
};