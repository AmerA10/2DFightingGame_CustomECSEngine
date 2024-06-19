#pragma once


#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"


class CollisionSystem : public System {


public:
	CollisionSystem() 
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(std::unique_ptr<EventBus>& eventBus) 
	{
		//TODO:
		//Check all entities that have a box collider to see if they are colliding with each other
		//Tip: Look for AABB collision Check. Axis Aligned Bounding Box

		//Maybe we can optimize this by storing the ids of the boxes that already collided in a map
		//so that we don't need to check it twice. If A collides with B then B collides with A so we do not need to check it twice

		std::vector entities = GetSystemEntities();
		if (entities.size() <= 1) {
			return;
		}

		for (Entity& en : entities) {
			en.GetComponent<BoxColliderComponent>().isCollliding = false;
		}


		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity entityA = *i;

			//Make sure we start where i starts otherwise we collide few times too many
			for (auto j = i; j != entities.end(); j++) {

				Entity entityB = *j;

				if (entityA.GetId() == entityB.GetId()) {
					continue;
				}

				const TransformComponent& transA = entityA.GetComponent<TransformComponent>();
				BoxColliderComponent& boxA = entityA.GetComponent<BoxColliderComponent>();
				const TransformComponent& transB = entityB.GetComponent<TransformComponent>();
				BoxColliderComponent& boxB = entityB.GetComponent<BoxColliderComponent>();



				if ((boxA.layer & boxB.mask).any() == false)
				{
					
					continue;
				}
				
				//Logger::Log("Collision with layer: " + boxA.layer.to_string());
				//Logger::Log("Collisio with mask: " + boxB.mask.to_string());
				//Logger::Log("Collision with layer and mask have bits common");
				
				bool didCollision = checkAABBCollision(transA.position.x + boxA.offset.x, transA.position.y + boxA.offset.y, boxA.width, boxA.height, transB.position.x + boxB.offset.x, transB.position.y + boxB.offset.y, boxB.width, boxB.height);

				if (didCollision)
				{
					boxA.isCollliding = true;
					boxB.isCollliding = true;
					//Logger::Log(std::to_string(entityA.GetId()) + " Collided with: " + std::to_string(entityB.GetId()));
					
					//Emmit an event 
					eventBus->EmitEvent<CollisionEvent>(entityA, entityB);

				} 


			}


		}

	}

	bool checkAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) 
	{
		if (   aX < bW + bX  
			&& aW + aX > bX 
			&& aY < bH + bY  
			&& aH + aY > bY )
		{
			return true;
		}
		else {
			return false;
		}
	}

};