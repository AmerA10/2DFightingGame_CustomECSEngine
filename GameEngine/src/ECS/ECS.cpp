#include "ECS.h"
#include "../logger/Logger.h"

int IComponent::nextId = 0;

//TODO: implement all the functions from ECS.h
int Entity::GetId() const {
	return id;
}

void Entity::Kill() {
	registry->KillEntity(*this);
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

Entity Registry::CreateEntity() 
{
	int entityId;

	//check and use an id from the freeIds deque

	if (freeIds.empty()) 
	{
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else 
	{
		//Reuse the id from the freeIds
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;
	//Make sure that the entityComponentSignatures vector can house this entity signatures
	entitiesToBeAdded.insert(entity);
	
	Logger::Log("Entity Created with id= " + std::to_string(entity.GetId()));
	
	return entity;
}

void Registry::KillEntity(Entity entity) 
{
	entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity) 
{
	const int entityId = entity.GetId();
	const Signature& entitySignature = entityComponentSignatures[entityId];

	//Try to match the system the components that the system
	//Requires to the components that the entity has
	//Loop through all the systems in the sysytems map and match the signature
	//Between the entity and the system

	for (std::pair<const std::type_index,std::shared_ptr<System>>& system : systemsMap) {
		const Signature& systemComponentSignature = system.second->GetComponentSignature();

		//we can perform a bitwise comparison to see if those two signatures are the same
		//if they are the same then they are the same as the systemComponentSignature
		bool IsInterested = (entitySignature & systemComponentSignature) == systemComponentSignature;

		if (IsInterested) {
			system.second->AddEntity(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) 
{

	for (auto system : systemsMap)
	{
		system.second->RemoveEntity(entity);
	}


}

void Registry::Update() {
	//Process entities waiting to be created
	for (Entity entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();


	//TODO: remove the entities that need to be removed
	for (Entity entity : entitiesToBeKilled) {
		RemoveEntityFromSystems(entity);

		//Make entity available for reuse

		Logger::Log("new free id: " + std::to_string(entity.GetId()));
		entityComponentSignatures[entity.GetId()].reset();
		
		freeIds.push_back(entity.GetId());

	}
	entitiesToBeKilled.clear();


}
