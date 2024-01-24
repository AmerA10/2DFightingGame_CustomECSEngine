#pragma once
#ifndef MOVEMENTSYSTEM_H

#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"

class MovementSystem : public System {

public:
	MovementSystem() {
		//We need to grab entities that the system is interested in
		//TODO: Require Component <TransformComponent>
		//
	}

	void Update() {
		//TOOD:
		//Loop all entities that the system is interested in
		//Here auto automatically gets the type of entity
		//Update entity position based on velocity in every frame of the game loop
		//for (auto entity : GetEntities()) {
			
		//}
	
	}


};


#endif // !MOVEMENTSYSTEM_H
