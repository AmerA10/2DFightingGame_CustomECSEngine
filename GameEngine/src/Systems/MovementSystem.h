#pragma once
#ifndef MOVEMENTSYSTEM_H

#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {

public:
	MovementSystem() {
		//We need to grab entities that the system is interested in
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		//

	}

	void Update(float deltaTime) {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
		for (auto& entity : GetSystemEntities()) {
			
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const RigidBodyComponent& rigidBody = entity.GetComponent <RigidBodyComponent>();
			
			//Need delta time
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;

			Logger::Log("Entity id = " + std::to_string(entity.GetId()) + " position is now (" + std::to_string(transform.position.x) + " ," + std::to_string(transform.position.y) + ")");
		
		}
	
	}


};


#endif // !MOVEMENTSYSTEM_H
