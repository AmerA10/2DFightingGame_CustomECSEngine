#pragma once
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BattleBoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/BattleCollisionEvent.h"

class BattleCollisionSystem : public System 
{


public:
	BattleCollisionSystem()
	{
		RequireComponent<BattleBoxColliderComponent>();
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

		std::vector<BoxColliderComponent> collidersAToCheck;
		std::vector<BoxColliderComponent> collidersBToCheck;



		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity entityA = *i;
			collidersAToCheck.clear();
			collidersAToCheck = entityA.GetComponent<BattleBoxColliderComponent>().boxes;
			const TransformComponent& transA = entityA.GetComponent<TransformComponent>();
			int isAFacingRight = entityA.GetComponent<BattleBoxColliderComponent>().isFacingRight == true? 1 : -1;

			//Make sure we start where i starts otherwise we collide few times too many
			for (auto j = i; j != entities.end(); j++) {

				Entity entityB = *j;

				if (entityA.GetId() == entityB.GetId()) {
					continue;
				}

				collidersBToCheck.clear();

				const TransformComponent& transB = entityB.GetComponent<TransformComponent>();
				collidersBToCheck = entityB.GetComponent<BattleBoxColliderComponent>().boxes;
				int isBFacingRight = entityB.GetComponent<BattleBoxColliderComponent>().isFacingRight == true? 1: -1;
				for (auto boxA = collidersAToCheck.begin(); boxA != collidersAToCheck.end(); boxA++)
				{
					for (auto boxB = collidersBToCheck.begin(); boxB != collidersBToCheck.end(); boxB++)
					{
						if ((boxA->layer & boxB->mask).any() == false)
						{
							continue;
						}
						bool didCollision = checkAABBCollision(transA.position.x + (boxA->offset.x * isAFacingRight), transA.position.y + boxA->offset.y, boxA->width, boxA->height, transB.position.x + (boxB->offset.x * isBFacingRight), transB.position.y + boxB->offset.y, boxB->width, boxB->height);

						if (didCollision)
						{
							boxA->isCollliding = true;
							boxB->isCollliding = true;

							//Emmit an event 
							eventBus->EmitEvent<BattleCollisionEvent>(entityA, entityB, *boxA,*boxB);


						}
					}
				}

			}
		}

	}
	bool checkAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
	{
		if (aX < bW + bX
			&& aW + aX > bX
			&& aY < bH + bY
			&& aH + aY > bY)
		{
			return true;
		}
		else {
			return false;
		}
	}

};