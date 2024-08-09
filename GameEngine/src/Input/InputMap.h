#pragma once
#include <unordered_map>
#include "./Input.h"
#include <SDL.h>

struct InputMap
{
	std::string inputMapId;

	std::unordered_map<SDL_Keycode, InputAction> inputMap;


};