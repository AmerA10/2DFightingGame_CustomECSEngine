#pragma once
#pragma once
#ifndef RENDERSYSTEM_H

#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>
#include <algorithm>
#include <vector> 

class RenderSystem : public System {

public:
	RenderSystem() {
		//We need to grab entities that the system is interested in
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		//

	}

	void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
		
		//TODO: Sort all the entities of our system by z-index. This is a horrible way of doing this every single frame
		// but we do this here for now

		std::vector<Entity> sortedEntities;

		for (Entity entity : GetSystemEntities())
		{
	
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

			bool isOutSideCameraView = transform.position.x + (sprite.width * transform.scale.x) < camera.x ||
				transform.position.x > camera.w + camera.x || 
				transform.position.y + (sprite.height * transform.scale.y) < camera.y || 
				transform.position.y > camera.h + camera.y;

			if (!sprite.isFixed && isOutSideCameraView)
			{
				continue;
			}

			sortedEntities.emplace_back(entity);
		}

		//This is extremely un-optimized
		std::sort(sortedEntities.begin(), sortedEntities.end(), [](const Entity& a, const Entity& b) {
				return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex;
			
		});

		//we can probably add a check to see if the position of the entity is within the camera rect or not and decide
		//to render it or not based on that
		
		for (auto entity : sortedEntities) 
		{
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

			//Set the source rectangle of our original sprite texture

			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect dstRect = { 
				static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
				static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)};

			
			//Draw the png texture based on sprite ID
			//SDL_RenderCopy(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect);
			//This function can take care of rotation, the NULL is where the center is, if NULL is passed
			//it will center based on half width and half height
			SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect, transform.rotation, NULL, sprite.flip);
			
		}

	}

	
};


#endif // !RenderSystem
