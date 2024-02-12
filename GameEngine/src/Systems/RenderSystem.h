#pragma once
#pragma once
#ifndef RENDERSYSTEM_H

#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>

class RenderSystem : public System {

public:
	RenderSystem() {
		//We need to grab entities that the system is interested in
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		//

	}

	void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore) {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
		for (auto& entity : GetSystemEntities()) {
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

			//Set the source rectangle of our original sprite texture
			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect dstRect = { 
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width * transform.scale.x, 
				sprite.height * transform.scale.y };

			
			//Draw the png texture based on sprite ID
			//SDL_RenderCopy(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect);
			//This function can take care of rotation, the NULL is where the center is, if NULL is passed
			//it will center based on half width and half height
			SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
			
		}

	}


};


#endif // !RenderSystem
