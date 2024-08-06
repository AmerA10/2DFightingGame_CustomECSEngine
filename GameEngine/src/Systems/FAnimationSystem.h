#pragma once


#include "../Components/FAnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include "../Animation/AnimationClip.h"
#include "../AssetStore/AssetStore.h"

class FAnimationSystem : public System {

public:

	FAnimationSystem() {
		RequireComponent<FAnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(const std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities())
		{
			FAnimationComponent& animComponent = entity.GetComponent<FAnimationComponent>();
			SpriteComponent& spriteComponent = entity.GetComponent<SpriteComponent>();

			if (animComponent.animClipId == "")
			{
				continue;
			}

			std::unique_ptr<FAnimationClip>& animClip = assetStore->GetFAnimationClip(animComponent.animClipId);
			std::vector<sol::function> funcsToPlay;
			std::unique_ptr<SpriteSheet>& sheet = assetStore->GetSpriteSheet(animClip->spriteSheetId);

			if (animComponent.animState == Stopped)
			{
				animComponent.animState = WaitingToPlay;
			}

			if (animComponent.animState == WaitingToPlay)
			{
				animComponent.localFrame = 0;
				animComponent.globalStartTime = (float)SDL_GetTicks();
				animComponent.animState = Playing;
				//need to change the sprite
				spriteComponent.assetId = animClip->spriteSheetId;
				animComponent.calledFuncs.clear();

			}

			animComponent.localFrame += 1;
			sol::function func;

			if (animClip->frameNumToSheetIndex.size() > 0)
			{
				auto it = animClip->frameNumToSheetIndex.lower_bound(animComponent.localFrame);


				if (it != animClip->frameNumToSheetIndex.begin())
				{
					--it;

				}

				if (animComponent.localFrame >= it->first)
				{

					animComponent.frameVal = it->second;

				}

			}

			if (sheet->indexToRect.size() > 0)
			{
				SDL_Rect newRect = sheet->indexToRect.at(animComponent.frameVal);
				spriteComponent.height = newRect.h;
				spriteComponent.width =  newRect.w;
				spriteComponent.srcRect = newRect;
			}

			//this is such a horrible system lmao
			//this code sucks, 5 if statement nested. clearly a rewrite is in order lmao
			if (animClip->frameNumToEvent.size() > 0)
			{
				auto it = animClip->frameNumToEvent.lower_bound(animComponent.localFrame);

				if (it != animClip->frameNumToEvent.begin())
				{
					--it;

				}

				if (animComponent.localFrame >= it->first)
				{
					if (it != animClip->frameNumToEvent.end())
					{

						auto serach = animClip->frameNumToEvent.find(it->first);

						if (serach != animClip->frameNumToEvent.end())
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

			if (animComponent.localFrame + 1 > animClip->duration)
			{
				animComponent.animState = WaitingToPlay;
			}

		}
	}


};