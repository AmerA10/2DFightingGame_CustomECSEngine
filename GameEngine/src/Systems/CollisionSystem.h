#pragma once


#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System {


public:
	CollisionSystem() 
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();

	}

	void Update() 
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


		for (Entity& entityA : entities) {

			for (Entity& entityB : entities) {

				if (entityA.GetId() == entityB.GetId()) {
					continue;
				}

				const TransformComponent& transA = entityA.GetComponent<TransformComponent>();
				BoxColliderComponent& boxA = entityA.GetComponent<BoxColliderComponent>();
				const TransformComponent& transB = entityB.GetComponent<TransformComponent>();
				BoxColliderComponent& boxB = entityB.GetComponent<BoxColliderComponent>();

				bool collisionEvent = checkAABBCollision(transA.position.x + boxA.offset.x, transA.position.y + boxA.offset.y, boxA.width, boxA.height, transB.position.x + boxB.offset.x, transB.position.y + boxB.offset.y, boxB.width, boxB.height);

				if (collisionEvent)
				{
					boxA.isCollliding = true;
					boxB.isCollliding = true;
					Logger::Log(std::to_string(entityA.GetId()) + " Collided with: " + std::to_string(entityB.GetId()));
					
					//Emmit an event in the future
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