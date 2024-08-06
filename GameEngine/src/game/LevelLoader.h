#pragma once

#include "../ECS/ECS.h"
#include <memory>
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include <SDL.h>
#include <sol/sol.hpp>

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry,const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level, std::unique_ptr<EventBus>& eventBus);

};