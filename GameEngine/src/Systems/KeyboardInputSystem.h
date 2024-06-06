#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/InputActionEvent.h"

//This keyboard input system sucks because it is not very universal
//

class KeyboardInputSystem : public System
{

public:

	KeyboardInputSystem() {

		//Ideally here really all we would want is a KeyboardControllerComponent
		//And then maybe another system can be a movement system that actually does the moving
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
	}

	void SubscribeToKeyInputEvent(std::unique_ptr<EventBus>& eventBus) 
	{
		eventBus->SubscribeToEvent<InputActionEvent>(this, &KeyboardInputSystem::OnKeyboardInput);
	}

	void OnKeyboardInput(InputActionEvent& event)
	{
		for (auto entity : GetSystemEntities()) 
		{
			const KeyboardControlledComponent& keyboardControlled = entity.GetComponent<KeyboardControlledComponent>();
			RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();


			if (event.action.inputActionName == "UP") 
			{

				rigidBody.velocity = keyboardControlled.upVel;
				sprite.srcRect.y = sprite.height * 0;
			}
			else if (event.action.inputActionName == "RIGHT") 
			{
				
				rigidBody.velocity = keyboardControlled.rightVel;
				sprite.srcRect.y = sprite.height * 1;
				

			}

			else if (event.action.inputActionName == "DOWN") 
			{

				rigidBody.velocity = keyboardControlled.downVel;
				sprite.srcRect.y = sprite.height * 2;
			}


			else if (event.action.inputActionName == "LEFT")
			{
				rigidBody.velocity = keyboardControlled.leftVel;
				sprite.srcRect.y = sprite.height * 3;
			
			}

		}
	}

};