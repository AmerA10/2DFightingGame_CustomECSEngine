#pragma once
#include "../ECS/ECS.h"

#include <SDL.h>
#include "../EventBus/EventBus.h"
#include "../Events/InputActionEvent.h"
#include "../Input/Input.h"
#include "../Components/InputActionReciever.h"

class InputBufferSystem : public System
{

public:

	InputBufferSystem()
	{
		//Best to process KeyboardInputRecievers Components?
		//Usually, input is handled via a central channel that then disperses it out to other
		//places of interest where by each interested entity listens to an event to process
		//Usually the inputs are mapped to specific actions and then each entity with an input component will then
		//be able to listen to the actions this is done via the command pattern

		//since input is funneled through our get input system really its our input system we have in Game.cpp that needs to be changed
	}


	void Update(std::unique_ptr<EventBus>& eventBus, SDL_Keycode& keyCode)
	{
		//here we can define our actions per say
		if (inputActionMapping.find(keyCode) != inputActionMapping.end())
		{
			eventBus->EmitEvent<InputActionEvent>(inputActionMapping.at(keyCode));
		}
	}

	void AddInputActionMapping(SDL_Keycode& keycode, std::string actionName)
	{
		InputAction actiontoAdd = { actionName };
		inputActionMapping.emplace(keycode, actiontoAdd);
	}

	//SDLK_SPACE -> FIRE
	std::unordered_map<SDL_Keycode, InputAction> inputActionMapping;

	
};
