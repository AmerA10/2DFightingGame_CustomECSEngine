#pragma once
#include "../ECS/ECS.h"

#include <SDL.h>
#include "../EventBus/EventBus.h"
#include "../Events/InputActionEvent.h"
#include "../Input/Input.h"
#include "../Components/InputBufferReceiverComponent.h"
#include "../AssetStore/AssetStore.h"

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
		RequireComponent<InputBufferReceiverComponent>();
	}


	void Update(std::unique_ptr<EventBus>& eventBus, SDL_Keycode& keyCode)
	{
		//here we can define our actions per say

		for (auto entity : GetSystemEntities())
		{
			entity.GetComponent<InputBufferReceiverComponent>().UpdateKeys(keys);
		}

		if (inputActionMapping.find(keyCode) != inputActionMapping.end())
		{
			eventBus->EmitEvent<InputActionEvent>(inputActionMapping.at(keyCode));
		}
	}

	void AddInputActionMapping(SDL_Keycode& keycode, std::string actionName, int scale)
	{
		InputAction actiontoAdd = { actionName, scale };
		inputActionMapping.emplace(keycode, actiontoAdd);
	}
	void AddKey(SDL_Keycode& keycode)
	{
		auto it = std::find(keys.begin(), keys.end(), keycode);
		//found it
		if (it != keys.end())
		{
			return;
		}
		keys.push_back(keycode);
		Logger::Log("Adding key: " + std::to_string(keycode));
	}
	void RemoveKey(SDL_Keycode& keycode)
	{
		auto it = std::find(keys.begin(), keys.end(), keycode);
		//found it
		if (it == keys.end())
		{
			return;
		}
		Logger::Log("Removing key: " + std::to_string(keycode));
		keys.erase(it);
	}

	void SetupInputComps(std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities())
		{
		
			InputBufferReceiverComponent& inputComp = entity.GetComponent<InputBufferReceiverComponent>();
			inputComp.SetupInputMap(assetStore->GetInputMap(inputComp.inputMapId));


		
		}
	}

	//SDLK_SPACE -> FIRE
	std::unordered_map<SDL_Keycode, InputAction> inputActionMapping;
	std::vector<SDL_Keycode> keys;

	
};
