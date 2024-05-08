#pragma once

#include "../ECS/ECS.h"
#include <SDL.h>
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
#include <glm/glm.hpp>

class ProjectilEmitterSystem : public System
{
public:
	ProjectilEmitterSystem()
	{
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(std::unique_ptr<Registry>& registry) 
	{
		for (auto entity : GetSystemEntities())
		{

			ProjectileEmitterComponent& projectileComponent = entity.GetComponent<ProjectileEmitterComponent>();
			const TransformComponent transform = entity.GetComponent<TransformComponent>();

			glm::vec2 projectileOrigin = transform.position;

			if (entity.HasComponent<SpriteComponent>())
			{
				const SpriteComponent spriteComp = entity.GetComponent<SpriteComponent>();
				projectileOrigin.x += (transform.scale.x *spriteComp.width )/ 2;
				projectileOrigin.y += (transform.scale.y * spriteComp.height)/ 2;

			}

			//TODO: 
			//Check if its time to re-emit a new projectile
			if (SDL_GetTicks() - projectileComponent.lastEmissionTime > projectileComponent.repeatFrequency)
			{
				Entity projectile = registry->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectileOrigin,glm::vec2(1.0,1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileComponent.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4);

				//
				projectileComponent.lastEmissionTime = SDL_GetTicks();
				
;			}

		}
	}

	Entity CreateProjectile()
	{
		
	}

};