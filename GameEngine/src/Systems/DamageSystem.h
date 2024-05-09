#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System 
{
	public:
		DamageSystem()
		{
			RequireComponent<BoxColliderComponent>();
		}
		DamageSystem(std::unique_ptr<EventBus>& eventBus) {
			this->SubscriberToEvents(eventBus);
		}

		void SubscriberToEvents(std::unique_ptr<EventBus>& eventBus ) 
		{
			eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
		}

		void onCollision(CollisionEvent& event)
		{
			Entity a = event.a;
			Entity b = event.b;
			Logger::Log("Collision Event: " + std::to_string(a.GetId())  +  " With: " + std::to_string(b.GetId()));
			
			if (a.BelongsToGroup("Projectile") && b.HasTag("Player"))
			{
				Logger::Log("Projectile Hit a Player");

			}
			if (b.BelongsToGroup("Projectile") && a.HasTag("Player"))
			{
				Logger::Log("Player Hit a Projectile");

			}
			if (a.BelongsToGroup("Projectile") && b.BelongsToGroup("Enemies"))
			{
				Logger::Log("Projectile Hit an enemy");
			}
			if (b.BelongsToGroup("Projectile") && a.BelongsToGroup("Enemies"))
			{
				Logger::Log("Projectile Hit a enemy");
			}
		
		}

		void Update() 
		{

		}
};