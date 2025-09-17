#pragma once

#include <engine.h>

#include "entities/enemies/PistolGuy.h"
#include "entities/enemies/UziGuy.h"
#include "entities/enemies/Reaper.h"
#include "entities/Player.h"
#include "entities/Bullet.h"

class SceneOverride : public Engine::Scene
{
public:
	SceneOverride() = default;
	~SceneOverride() = default;

	// TODO: Not this, this sucks
	virtual void UpdateScene(Engine::Timestep ts) override
	{
		auto PlayerComponentView = m_Registry.view<PlayerComponent>();
		for (auto entity : PlayerComponentView)
		{
			PlayerComponent& PC = PlayerComponentView.get<PlayerComponent>(entity);
			PC.OnUpdate(ts);
		}

		auto PistolGuyComponentView = m_Registry.view<PistolGuyComponent>();
		for (auto entity : PistolGuyComponentView)
		{
			PistolGuyComponent& PGC = PistolGuyComponentView.get<PistolGuyComponent>(entity);
			PGC.OnUpdate(ts);
		}

		auto UziGuyComponentView = m_Registry.view<UziGuyComponenet>();
		for (auto entity : UziGuyComponentView)
		{
			UziGuyComponenet& UGC = UziGuyComponentView.get<UziGuyComponenet>(entity);
			UGC.OnUpdate(ts);
		}

		auto ReaperComponentView = m_Registry.view<ReaperComponent>();
		for (auto entity : ReaperComponentView)
		{
			ReaperComponent& RC = ReaperComponentView.get<ReaperComponent>(entity);
			RC.OnUpdate(ts);
		}

		auto BulletComponentView = m_Registry.view<BulletComponenet>();
		for (auto entity : BulletComponentView)
		{
			BulletComponenet& BC = BulletComponentView.get<BulletComponenet>(entity);
			BC.OnUpdate(ts);
		}

		Engine::Scene::UpdateScene(ts);
	}
};