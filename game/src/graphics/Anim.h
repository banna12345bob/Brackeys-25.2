#pragma once

#include <engine.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Frame {
	std::array<glm::vec2, 4> uvs;
	float duration;
};

class Anim {
public: 
	static std::vector<Engine::Ref<Anim>> LoadAnims(std::string path);

	std::string name;
	std::vector<Frame> frames;
	Engine::Ref<Engine::Texture2D> texture;
private:
	static json readJson(std::string path);

	Anim(std::string name, std::vector<Frame> frames, Engine::Ref<Engine::Texture2D> texture);
};

class Animator {
public:
	Engine::Ref<Anim> anim;
	float progress = 0.0;

	Animator(Engine::Ref<Anim> anim);

	std::array<glm::vec2, 4> Get();
};