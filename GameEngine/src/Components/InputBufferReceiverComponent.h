#pragma once
#include <string>
#include "../Input/InputMap.h"
#include <SDL.h>
struct InputBufferReceiverComponent
{
	std::string inputMapId;

	InputMap inputMap;

	std::vector<SDL_Keycode> keys;

public:
	InputBufferReceiverComponent(const std::string& inputMapId = "", InputMap inputMap = {}, std::vector<SDL_Keycode> keys = {})
	{
		this->inputMapId = inputMapId;
		this->inputMap = inputMap;
		this->keys = keys;
	}

	void UpdateKeys(std::vector<SDL_Keycode>& keysToAdd)
	{
		this->keys.clear();
		this->keys.insert(this->keys.end(), keysToAdd.begin(), keysToAdd.end());
	}

	InputAction GetCurrentAction()
	{

		InputAction result = { "NONE" };

		if (keys.size() <= 0)
		{
			return result;
		}

		const SDL_Keycode& lastKey = keys[keys.size() - 1];
		if (inputMap.inputMap.find(lastKey) != inputMap.inputMap.end())
		{
			result = inputMap.inputMap.at(lastKey);
		}
		
		return result;
	}

	void SetupInputMap(const std::unique_ptr<InputMap>& inputMapToAdd)
	{
		//this is not the way this is supposed to work lmao this should be a shared pointer or something
		this->inputMap.inputMapId = inputMapToAdd->inputMapId;
		this->inputMap.inputMap = inputMapToAdd->inputMap;
	}

};