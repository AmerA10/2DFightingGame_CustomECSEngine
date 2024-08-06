#pragma once

#include "../EventBus/Event.h"
#include <SDL.h>

class KeyboardInputEvent : public Event
{
	public:
		std::string keyName;
		SDL_Keycode keyCode;

		KeyboardInputEvent(SDL_Keycode keyCode)
		{
			keyName = SDL_GetKeyName(keyCode);
			this->keyCode = keyCode;
		}
};