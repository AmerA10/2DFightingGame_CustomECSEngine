#pragma once
#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include "../Input/Input.h"


class InputActionEvent : public Event
{
public:
	InputAction action;
	
	InputActionEvent(InputAction inputAction)
	{
		action = inputAction;
	}

	bool operator == (const std::string& other) const { return action.inputActionName == other; };

};

