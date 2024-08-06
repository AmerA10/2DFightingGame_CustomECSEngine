#pragma once

#include <SDL.h>
#include <sol/sol.hpp>
#include "../ECS/ECS.h"
#include <memory>

class InputLoader
{
public:

	InputLoader();
	~InputLoader();

	void LoadInput(sol::state& lua, const std::unique_ptr<Registry>& registry);

};