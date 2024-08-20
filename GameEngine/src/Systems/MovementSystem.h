#pragma once
#ifndef MOVEMENTSYSTEM_H

#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/FighterComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"
#include <memory>

class MovementSystem : public System {

public:
	MovementSystem() {
		//We need to grab entities that the system is interested in
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<FighterComponent>();
		//

	}

	void Update(const std::unique_ptr<Registry>& registry, float deltaTime) {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
	

		for (auto& entity : GetSystemEntities()) {
			
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const RigidBodyComponent& rigidBody = entity.GetComponent <RigidBodyComponent>();
			const FighterComponent& fighter = entity.GetComponent<FighterComponent>();
			
			//Need delta time
			transform.position.x += fighter.currentActionVelocity * deltaTime;
			

			bool isEntityOutSideMap =	transform.position.x < 0 ||
										transform.position.x > Game::mapWidth ||
										transform.position.y < 0 ||
										transform.position.y > Game::mapHeight;
			if (isEntityOutSideMap && !entity.HasTag("Player"))
			{
				entity.Kill();
			}

			if (entity.HasTag("Player"))
			{
				transform.position.x = transform.position.x < 10 ? 10 : transform.position.x;
				transform.position.x = transform.position.x > Game::mapWidth - 50 ? Game::mapWidth - 50: transform.position.x;
				transform.position.y = transform.position.y < 10 ? 10 : transform.position.y;
				transform.position.y = transform.position.y > Game::mapHeight - 350 ? Game::mapHeight - 350: transform.position.y;
			}
		
		}
	
	}

	void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollisionEvent);
	}

	void OnCollisionEvent(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;

		if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles"))
		{

			OnEnemyHitObstacle(a);
			

		}

		if (b.BelongsToGroup("enemies") && a.BelongsToGroup("obstacles"))
		{

			OnEnemyHitObstacle(b);

		}
	}

	void OnEnemyHitObstacle(Entity enemy)
	{

		if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>())
		{
			RigidBodyComponent& rb = enemy.GetComponent<RigidBodyComponent>();

			SpriteComponent& sp = enemy.GetComponent<SpriteComponent>();


			if (rb.velocity.x != 0)
			{
				rb.velocity.x *= -1;

				sp.flip = sp.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

			}

			if (rb.velocity.y != 0)
			{
				rb.velocity.y *= -1;
				sp.flip = sp.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;

			}

		}



		



		

	}
};


#endif // !MOVEMENTSYSTEM_H
