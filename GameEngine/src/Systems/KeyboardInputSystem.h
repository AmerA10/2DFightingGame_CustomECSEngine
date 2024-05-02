#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

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
		eventBus->SubscribeToEvent<KeyboardInputEvent>(this, &KeyboardInputSystem::OnKeyboardInput);
	}

	void OnKeyboardInput(KeyboardInputEvent& event)
	{
		for (auto entity : GetSystemEntities()) 
		{
			const KeyboardControlledComponent& keyboardControlled = entity.GetComponent<KeyboardControlledComponent>();
			RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

			Logger::Log("Recieved input: " + event.keyName);

			switch (event.keyCode) {
				case (SDLK_UP):
					rigidBody.velocity = keyboardControlled.upVel;
					sprite.srcRect.y = sprite.height * 0;
					break;
				case (SDLK_RIGHT):
					rigidBody.velocity = keyboardControlled.rightVel;
					sprite.srcRect.y = sprite.height * 1;
					break;
				case (SDLK_DOWN):

					rigidBody.velocity = keyboardControlled.downVel;
					sprite.srcRect.y = sprite.height * 2;
					break;
				case (SDLK_LEFT):
					rigidBody.velocity = keyboardControlled.leftVel;
					sprite.srcRect.y = sprite.height * 3;
					break;
			}

		}
	}

};