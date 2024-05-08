#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class RenderDebugSystem : public System {

public:
	RenderDebugSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}


	void Update(SDL_Renderer* renderer, const SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities()) 
		{
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const BoxColliderComponent& boxCollider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect collisionRect = {
				static_cast<int>(transform.position.x + boxCollider.offset.x - camera.x),
				static_cast<int>(transform.position.y + boxCollider.offset.y - camera.y),
				static_cast<int>(boxCollider.width),
				static_cast<int>(boxCollider.height)
			};

			
			if (boxCollider.isCollliding) {

				SDL_SetRenderDrawColor(renderer, 255, 200, 0,1);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 200, 200, 200,1);

			}

			SDL_RenderDrawRect(renderer, &collisionRect);
			

		}
	}


};