#pragma once


#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <glm/glm.hpp>

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


			if (animComponent.animState == Stopped)
			{
				continue;
			}

			if (animComponent.animState == WaitingToPlay)
			{
				animComponent.localTime = 0.0;
				animComponent.globalStartTime = SDL_GetTicks();
				animComponent.animState = Playing;
				//need to change the sprite
				spriteComponent.assetId = 

			}
	
				
			if (animComponent.numLoops < 0)
			{
				animComponent.localTime = fmod((SDL_GetTicks() - animComponent.globalStartTime) * animComponent.playbackRate  ,animComponent.duration);
				
					
			}
			//finite
			else if(animComponent.numLoops == 0)
				{
				animComponent.localTime = glm::clamp((SDL_GetTicks() - animComponent.globalStartTime) * animComponent.playbackRate, 0.0f, animComponent.duration);

			}
			else 
			{
				animComponent.localTime = fmod(glm::clamp((SDL_GetTicks() - animComponent.globalStartTime) * animComponent.playbackRate, 0.0f, animComponent.duration * animComponent.numLoops), animComponent.duration);

			}


			spriteComponent.srcRect.x = animComponent.currentFrame * spriteComponent.width;
			

		}
	}


};