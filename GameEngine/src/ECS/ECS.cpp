#include "ECS.h"
#include "../logger/Logger.h"

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

Entity Registry::CreateEntity() {
	int entityId;

	entityId = numEntities++;

	Entity entity(entityId);

	//Make sure that the entityComponentSignatures vector can house this entity signatures
	if (entityId >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(entityId + 1);
	}

	entitiesToBeAdded.insert(entity);
	
	Logger::Log("Entity Created with id= " + std::to_string(entity.GetId()));
	
	return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
	const int entityId = entity.GetId();
	const Signature& entitySignature = entityComponentSignatures[entityId];

	//Try to match the system the components that the system
	//Requires to the components that the entity has
	//Loop through all the systems in the sysytems map and match the signature
	//Between the entity and the system

	for (std::pair<const std::type_index,System*>& system : systemsMap) {
		const Signature& systemComponentSignature = system.second->GetComponentSignature();

		//we can perform a bitwise comparison to see if those two signatures are the same
		//if they are the same then they are the same as the systemComponentSignature
		bool IsInterested = (entitySignature & systemComponentSignature) == systemComponentSignature;

		if (IsInterested) {
			system.second->AddEntity(entity);
		}
	}
}


void Registry::Update() {
	for (Entity entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();


	//TODO: remove the entities that need to be removed

}
