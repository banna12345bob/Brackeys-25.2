#include "Anim.h"

json Anim::readJson(std::string filename)
{
	std::string fileExtension = filename.substr(filename.find_last_of(".") + 1);
	EG_CORE_ASSERT(fileExtension == "json", "File is not json");
	std::ifstream f(filename);
	if (!(f)) {
		return false;
	}
	json data = json::parse(f);
	return data;
}

Anim::Anim(std::string name, std::vector<Frame> frames, Engine::Ref<Engine::Texture2D> texture, bool loop) {
	this->name = name;
	this->frames = frames;
	this->texture = texture;
	this->loop = loop;
}

std::unordered_map<std::string, Engine::Ref<Anim>> Anim::LoadAnims(std::string path) {
	json data = readJson(path);
	std::unordered_map<std::string, Engine::Ref<Anim>> allAnims = {};

	for (auto item = data.begin(); item != data.end(); ++item) {
		std::string prefix = item.key();
		json properties = item.value();

		std::string imagePath = properties["image"];
		Engine::Ref<Engine::Texture2D> texture = Engine::Texture2D::Create(imagePath);
		float textureWidth = (float)texture.get()->getWidth();
		float textureHeight = (float)texture.get()->getHeight();

		int tileSizeX = properties["tileSize"]["x"];
		int tileSizeY = properties["tileSize"]["y"];
		int tileNumX = properties["numTiles"]["x"];
		int tileNumY = properties["numTiles"]["y"];

		// Create unordered map of defined frames
		json framesJson = properties["frames"];
		std::unordered_map<std::string, Engine::Ref<Engine::Texture2D>> frameMap = {};
		for (auto framePair = framesJson.begin(); framePair != framesJson.end(); ++framePair) {
			std::string name = framePair.key();
			json value = framePair.value();
			int frameX = value["x"];
			int frameY = value["y"];

			Engine::Ref<Engine::Texture2D> frameTex = Engine::SubTexture2D::CreateFromCoords(texture, { frameX, frameY }, { tileSizeX, tileSizeY });

			//glm::vec2 tl = { (frameX * tileSizeX) / textureWidth, (frameY * tileSizeY) / textureHeight };
			//glm::vec2 tr = { ((frameX + 1) * tileSizeX) / textureWidth, (frameY * tileSizeY) / textureHeight };
			//glm::vec2 br = { ((frameX + 1) * tileSizeX) / textureWidth, ((frameY + 1) * tileSizeY) / textureHeight };
			//glm::vec2 bl = { (frameX * tileSizeX) / textureWidth, ((frameY + 1) * tileSizeY) / textureHeight };
			//std::array<glm::vec2, 4> uvs = { tl, tr, br, bl };

			frameMap.insert({ name, frameTex });
		}

		// Create anims using predefined frames
		json anims = properties["anims"];
		for (auto animPair = anims.begin(); animPair != anims.end(); ++animPair) {
			std::string name = prefix + "_" + animPair.key();
			std::vector<Frame> frames = {};
			json value = animPair.value();
			for (auto framePair = value.begin(); framePair != value.end(); ++framePair) {
				json frameValue = framePair.value();
				std::string frameName = frameValue["name"];
				Engine::Ref<Engine::Texture2D> subTexture = frameMap[frameName];
				int duration_ms = frameValue["dur"];
				float duration = (float)duration_ms / 1000.0f;
				frames.push_back(Frame{ subTexture, duration });
			}

			EG_CORE_INFO("Deserialized Anim {} ({} frame(s))", name, frames.size());
			// Figure out how to properally decide if animation loops for now loop = true
			Anim* anim = new Anim(name, frames, texture, true);
			allAnims[name] = Engine::Ref<Anim>(anim);
			//allAnims.push_back(Engine::Ref<Anim>(anim));
		}

	}

	return allAnims;
}

Animator::Animator(Engine::Ref<Anim> anim) {
	this->anim = anim;
	this->progress = 0.0f;
}

Engine::Ref<Engine::Texture2D> Animator::Get() {
	Anim* anim = this->anim.get();
	
	float timePassed = 0.0f;
	for (int i = 0; i < anim->frames.size(); ++i) {
		if (timePassed >= this->progress) {
			return anim->frames[i].frameTexture;
		}

		timePassed += anim->frames[i].duration;
	}

	if(anim->loop)
		progress = 0.f;
	// Return last if finished
	int last = anim->frames.size() - 1;
	return anim->frames[last].frameTexture;
}