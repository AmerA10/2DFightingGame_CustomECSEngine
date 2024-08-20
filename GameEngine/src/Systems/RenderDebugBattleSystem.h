#pragma once

#include "../ECS/ECS.h"
#include "../Components/BattleBoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class RenderDebugBattleSystem : public System {

	std::bitset<32> hitBox   = std::bitset<32>("10000000000000000000000000000000");
	std::bitset<32> hurtBox  = std::bitset<32>("01000000000000000000000000000000");
	std::bitset<32> touchBox = std::bitset<32>("00100000000000000000000000000000");

public:
	RenderDebugBattleSystem() {
		RequireComponent<BattleBoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}


	void Update(SDL_Renderer* renderer, const SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities())
		{
			const TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const BattleBoxColliderComponent& boxCollider = entity.GetComponent<BattleBoxColliderComponent>();
			int isFacingRight = boxCollider.isFacingRight == true ? 1 : -1;
			for (auto box = boxCollider.boxes.begin(); box != boxCollider.boxes.end(); box++)
			{
				SDL_Rect collisionRect = {
					static_cast<int>(transform.position.x + (box->offset.x * isFacingRight) - camera.x),
					static_cast<int>(transform.position.y + box->offset.y - camera.y),
					static_cast<int>(box->width),
					static_cast<int>(box->height)
				};


				if (box->layer ==hitBox) {

					SDL_SetRenderDrawColor(renderer, 255, 20, 0, 1);
				}
				else if(box->layer == hurtBox){
					SDL_SetRenderDrawColor(renderer, 20, 255, 0, 1);

				}
				else if (box->layer == touchBox)
				{
					SDL_SetRenderDrawColor(renderer, 200, 200, 200, 1);

				}
				else {

				}

				SDL_RenderDrawRect(renderer, &collisionRect);
			}



		}
	}


};