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


			if (event.action.inputActionName == "MOVEY") 
			{
				
				rigidBody.velocity = keyboardControlled.yVel;
				rigidBody.velocity.y *= event.action.scale;

				if (event.action.scale >= 1)
				{
				sprite.srcRect.y = sprite.height * 0;

				}
				else
				{
					sprite.srcRect.y = sprite.height * 2;

				}

			}
			else if (event.action.inputActionName == "MOVEX") 
			{
				
				rigidBody.velocity = keyboardControlled.xVel;
				rigidBody.velocity.x *= event.action.scale;

				if (event.action.scale >= 1)
				{
					sprite.srcRect.y = sprite.height * 1;

				}
				else
				{
					sprite.srcRect.y = sprite.height * 3;

				}
				

			}
		}
	}

};