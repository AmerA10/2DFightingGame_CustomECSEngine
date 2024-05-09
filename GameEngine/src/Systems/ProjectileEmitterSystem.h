#pragma once

#include "../ECS/ECS.h"
#include <SDL.h>
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"
#include <glm/glm.hpp>

class ProjectilEmitterSystem : public System
{
public:
	ProjectilEmitterSystem()
	{
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<TransformComponent>();
	}

	void SubscribeToKeyInputEvent(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<KeyboardInputEvent>(this, &ProjectilEmitterSystem::CreateProjectileEvent);
	}


	void Update(std::unique_ptr<Registry>& registry) 
	{
		for (auto entity : GetSystemEntities())
		{

			

			ProjectileEmitterComponent& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const TransformComponent transform = entity.GetComponent<TransformComponent>();

			if (!projEmitter.useFrequency)
			{
				continue;
			}

			glm::vec2 projectileOrigin = transform.position;

			if (entity.HasComponent<SpriteComponent>())
			{
				const SpriteComponent spriteComp = entity.GetComponent<SpriteComponent>();
				projectileOrigin.x += (transform.scale.x * spriteComp.width )/ 2;
				projectileOrigin.y += (transform.scale.y * spriteComp.height)/ 2;

			}

			//TODO: 
			//Check if its time to re-emit a new projectile
			if (SDL_GetTicks() - projEmitter.lastEmissionTime > projEmitter.repeatFrequency)
			{
				Entity projectile = registry->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectileOrigin,glm::vec2(4.0,4.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4, glm::vec2(4,4));
				projectile.AddComponent<ProjectileComponent>(projEmitter.isFriendly, projEmitter.hitPercentDamage, projEmitter.projectileDuration);
				//
				projEmitter.lastEmissionTime = SDL_GetTicks();
				
				projectile.Group("Projectile");

;			}

		}
	}

	void CreateProjectileEvent(KeyboardInputEvent& event)
	{
		if (event.keyCode == SDLK_SPACE)
		{
			for (auto entity : GetSystemEntities())
			{
				if (entity.HasComponent<CameraFollowComponent>()) 
				{
					const RigidBodyComponent rbc = entity.GetComponent<RigidBodyComponent>();
					const TransformComponent tc = entity.GetComponent<TransformComponent>();
					ProjectileEmitterComponent& pec = entity.GetComponent<ProjectileEmitterComponent>();
					Entity projectile = entity.registry->CreateEntity();
					
					glm::vec2 projectileOrigin = tc.position;

					if (entity.HasComponent<SpriteComponent>())
					{
						const SpriteComponent spriteComp = entity.GetComponent<SpriteComponent>();
						projectileOrigin.x += (tc.scale.x * spriteComp.width) / 2;
						projectileOrigin.y += (tc.scale.y * spriteComp.height) / 2;

					}

					glm::vec2 vel = rbc.velocity;
					glm::vec2 projVel = glm::normalize(vel);
					projVel = projVel * (float)pec.projSpeed;
					
					projectile.AddComponent<TransformComponent>(projectileOrigin, glm::vec2(6.0, 6.0), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projVel);
					projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
					projectile.AddComponent<BoxColliderComponent>(4, 4, glm::vec2(6,6));
					projectile.AddComponent<ProjectileComponent>(pec.isFriendly, pec.hitPercentDamage, pec.projectileDuration);
					projectile.Group("Projectile");


				}
			}
		}
	}

};