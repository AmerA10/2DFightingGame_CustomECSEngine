#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"


class KeyboardInputSystem : public System
{

public:
	void SubscribeToKeyInputEvent(std::unique_ptr<EventBus>& eventBus) 
	{
		eventBus->SubscribeToEvent<KeyboardInputEvent>(this, &KeyboardInputSystem::OnKeyboardInput);
	}

	void OnKeyboardInput(KeyboardInputEvent& event)
	{
		Logger::Log("KeyBoard input event: " + event.keyname);
	}

};