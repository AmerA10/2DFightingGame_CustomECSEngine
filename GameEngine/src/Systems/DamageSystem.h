#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Events/HealthUpdatedEvent.h"

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
			//Logger::Log("Collision Event: " + std::to_string(a.GetId())  +  " With: " + std::to_string(b.GetId()));
			
			if (a.BelongsToGroup("Projectile") && b.HasTag("Player"))
			{
				//Logger::Log("Projectile Hit a Player");
				OnProjectileHitsPlayer(a, b);

			}
			if (b.BelongsToGroup("Projectile") && a.HasTag("Player"))
			{
				//Logger::Log("Player Hit a Projectile");
				OnProjectileHitsPlayer(b, a);

			}
			if (a.BelongsToGroup("Projectile") && b.BelongsToGroup("Enemies"))
			{
				//Logger::Log("Projectile Hit an enemy");
				OnProjectileHitsEnemy(a, b);
			}
			if (b.BelongsToGroup("Projectile") && a.BelongsToGroup("Enemies"))
			{
				//Logger::Log("Projectile Hit a enemy");
				OnProjectileHitsEnemy(b, a);

			}
		
		}


		void OnProjectileHitsPlayer(Entity projectile, Entity player)
		{
			const ProjectileComponent projComp = projectile.GetComponent<ProjectileComponent>();
			

			if (!player.HasComponent<HealthComponent>())
			{
				return;
			}

			HealthComponent& playerHealthComp = player.GetComponent<HealthComponent>();


			
			if (projComp.isFriendly == false)
			{
				playerHealthComp.health -= projComp.hitPercentDamage;

				if (playerHealthComp.health <= 0)
				{
					player.Kill();
				}
				projectile.Kill();
			}

			
		
		}
		void OnProjectileHitsEnemy(Entity projectile, Entity enemy)
		{
			const ProjectileComponent projComp = projectile.GetComponent<ProjectileComponent>();

			if (!enemy.HasComponent<HealthComponent>())
			{
				return;
			}

			HealthComponent& enemyHealthComp = enemy.GetComponent<HealthComponent>();

			if (projComp.isFriendly == true)
			{
				enemyHealthComp.health -= projComp.hitPercentDamage;
				if (enemyHealthComp.health <= 0)
				{
					enemy.Kill();
				}
				projectile.Kill();
			}

			
		}
		

};