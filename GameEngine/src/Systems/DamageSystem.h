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
			Logger::Log("Collision Event ");
			//event.a.Kill();
			//event.b.Kill();
		}

		void Update() 
		{

		}
};