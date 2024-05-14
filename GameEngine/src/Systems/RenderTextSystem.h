#pragma once

#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include <SDL.h>
#include <memory>

class RenderTextSystem : public System
{
public:
	RenderTextSystem()
	{
		RequireComponent<TextLabelComponent>();
	}

	void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities())
		{
			const TextLabelComponent textLabelComponent = entity.GetComponent<TextLabelComponent>();

			//To create fonts in SDL we create a surface pointer
			SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont(textLabelComponent.assetId), textLabelComponent.text.c_str(), textLabelComponent.color);
			
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			SDL_FreeSurface(surface);


			int labelWidth = 0;
			int labelHeight = 0;

			//We do not know the vales of wdith and height of the font so we have to query it in a way
			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect dstRect = {
				static_cast<int>(textLabelComponent.position.x - (textLabelComponent.isFixed ? 0 : camera.x)),
				static_cast<int>(textLabelComponent.position.y - (textLabelComponent.isFixed ? 0 : camera.y)),
				labelWidth,
				labelHeight

			};

			SDL_RenderCopy(renderer, texture, NULL, &dstRect);

			SDL_DestroyTexture(texture);

		}
	}
};