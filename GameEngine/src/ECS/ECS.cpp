#include "ECS.h"

int IComponent::nextId = 0;

//TODO: implement all the functions from ECS.h
int Entity::GetId() const {
	return id;
}

void System::AddEntity(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntity(Entity entity) {

	//You could do a for loop however there is a better way
	//This will be using an iterator and an anynomous function
	//This is the most fucked way of going about a for loop function I've ever seen in my entire life
	
	//The advantages of using erase however is that it is compatible with any std container
	//Map, unordered map etc..
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
			return entity == other;
		}
	),entities.end());

}

std::vector<Entity> System::GetSystemEntities() const {

	return entities;

}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}


