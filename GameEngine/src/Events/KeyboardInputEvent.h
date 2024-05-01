#pragma once

#include "../EventBus/Event.h"
#include <SDL.h>

class KeyboardInputEvent : public Event
{
	public:
		std::string keyname;

		KeyboardInputEvent(SDL_Keycode sym)
		{
			keyname = SDL_GetKeyName(sym);
		}
};