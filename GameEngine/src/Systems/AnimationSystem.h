#pragma once


#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

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

			std::shared_ptr<SpriteAnimationClip> animClip = assetStore->GetAnimationClip(animComponent.animClipId);

			std::vector<sol::function> funcsToPlay;
			

			if (animComponent.animState == Stopped)
			{
				continue;
			}

			if (animComponent.animState == WaitingToPlay)
			{
				animComponent.localTime = 0.0f;
				animComponent.globalStartTime = (float)SDL_GetTicks();
				animComponent.animState = Playing;
				//need to change the sprite
				spriteComponent.assetId = animClip->spriteSheetId;
				animComponent.calledFuncs.clear();
				Logger::Log("waiting to play anim");


			}
	
			float time = (float)(SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f;
			std::cout << "Time: " << time << '\n';


			if (animComponent.numLoops < 0)
			{
				animComponent.localTime = fmod(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f)  * animComponent.playbackRate  ,animClip->duration);
				
				
				if (time >= animClip->duration)
				{
					animComponent.animState = WaitingToPlay;
				}
			}

			//finite
			else if(animComponent.numLoops == 0)
			{
				animComponent.localTime = glm::clamp(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f) * animComponent.playbackRate, 0.0f, animClip->duration);

				if (time >= animClip->duration)
				{
					animComponent.animState = Stopped;
				}

			}
			else 
			{
				animComponent.localTime = fmod(glm::clamp(((SDL_GetTicks() - animComponent.globalStartTime) / 1000.0f) * animComponent.playbackRate, 0.0f, animClip->duration * animComponent.numLoops), animClip->duration);
			}

			

			sol::function func;

			if (animClip->timeToFrameVal.size() > 0)
			{
				auto it = animClip->timeToFrameVal.lower_bound(animComponent.localTime);


				if (it != animClip->timeToFrameVal.begin())
				{
					--it;

				}

				if (animComponent.localTime >= it->first)
				{

					animComponent.frameVal = glm::clamp(it->second, 0, (spriteComponent.numHCuts * spriteComponent.numVCuts) - 1);


				}


			}


			//this is such a horrible system lmao
			//this code sucks, 5 if statement nested. clearly a rewrite is in order lmao
			if (animClip->timeToEvent.size() > 0)
			{
				auto it = animClip->timeToEvent.lower_bound(animComponent.localTime);

				if (it != animClip->timeToEvent.begin())
				{
					--it;

				}

				if (animComponent.localTime >= it->first)
				{
					if (it != animClip->timeToEvent.end())
					{

						auto serach = animClip->timeToEvent.find(it->first);

						if (serach != animClip->timeToEvent.end())
						{
							sol::function func = it->second;


							if (!animComponent.HasPlayedClip(func))
							{
								func(entity);
								animComponent.calledFuncs.emplace_back(func);
							}

						}
					}
				}





			}

			spriteComponent.srcRect.x = (animComponent.frameVal * spriteComponent.width) % (spriteComponent.width * spriteComponent.numVCuts);
			spriteComponent.srcRect.y = glm::floor((animComponent.frameVal / spriteComponent.numVCuts)) * spriteComponent.height ;

			

		}
	}


};