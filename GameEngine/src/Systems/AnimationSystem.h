#pragma once


#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include <SDL.h>

class AnimationSystem : public System {

public:

	AnimationSystem() {
		RequireComponent<AnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update() 
	{
		for (auto entity : GetSystemEntities()) 
		{
			AnimationComponent& animComponent = entity.GetComponent<AnimationComponent>();
			SpriteComponent& spriteComponent = entity.GetComponent<SpriteComponent>();

			animComponent.currentFrame = ((SDL_GetTicks() - animComponent.startTime)
				* animComponent.frameSpeedRate / 1000) % animComponent.numFrames;

			spriteComponent.srcRect.x = animComponent.currentFrame * spriteComponent.width;
			

		}
	}


};