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

	void Update(const std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities()) 
		{
			AnimationComponent& animComponent = entity.GetComponent<AnimationComponent>();
			SpriteComponent& spriteComponent = entity.GetComponent<SpriteComponent>();

			if (animComponent.animClipId == "")
			{
				continue;
			}

			std::shared_ptr< SpriteAnimationClip> animClip = assetStore->GetAnimationClip(animComponent.animClipId);
			if (animComponent.animState == Stopped)
			{
				continue;
			}

			if (animComponent.animState == WaitingToPlay)
			{
				animComponent.localTime = 0.0;
				animComponent.globalStartTime = (float)SDL_GetTicks();
				animComponent.animState = Playing;
				//need to change the sprite
				spriteComponent.assetId = animClip->spriteSheetId;

			}
	
				
			if (animComponent.numLoops < 0)
			{
				animComponent.localTime = fmod(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f)  * animComponent.playbackRate  ,animClip->duration);
					
			}
			//finite
			else if(animComponent.numLoops == 0)
			{
				animComponent.localTime = glm::clamp(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f) * animComponent.playbackRate, 0.0f, animClip->duration);

				if ((SDL_GetTicks() - animComponent.globalStartTime) >= animClip->duration)
				{
					animComponent.animState = Stopped;
				}

			}
			else 
			{
				animComponent.localTime = fmod(glm::clamp(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f) * animComponent.playbackRate, 0.0f, animClip->duration * animComponent.numLoops), animClip->duration);
			}

			int frameVal = 0;

			if (animClip->timeToFrameVal.size() > 0)
			{
				auto it = animClip->timeToFrameVal.lower_bound(animComponent.localTime);


				if (it != animClip->timeToFrameVal.begin())
				{
					--it;

				}


				frameVal = glm::clamp(it->second, 0, (spriteComponent.numHCuts * spriteComponent.numVCuts) - 1);
			}



			spriteComponent.srcRect.x = (frameVal * spriteComponent.width) % (spriteComponent.width * spriteComponent.numVCuts);
			spriteComponent.srcRect.y = glm::floor((frameVal / spriteComponent.numVCuts)) * spriteComponent.height ;

			

		}
	}


};