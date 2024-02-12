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

	void Update(SDL_Renderer* renderer) {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
		for (auto& entity : GetSystemEntities()) {
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

			SDL_Rect objRect;
			objRect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), sprite.width, sprite.height };

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 2555);
			SDL_RenderFillRect(renderer, &objRect);
		}

	}


};


#endif // !RenderSystem
