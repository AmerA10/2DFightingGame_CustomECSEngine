#include "ECS.h"
#include "../logger/Logger.h"

int IComponent::nextId = 0;

//TODO: implement all the functions from ECS.h
int Entity::GetId() const 
{
	return id;
}

void Entity::Kill() 
{
	registry->KillEntity(*this);
}

void Entity::Tag(const std::string& tag) 
{
	registry->TagEntity(*this, tag);
}

void Entity::Group(const std::string& group)
{
	registry->GroupEntity(*this, group);
}

bool Entity::HasTag(const std::string& tag) const
{
	return registry->EntityHasTag(*this, tag);
}

bool Entity::BelongsToGroup(const std::string& group) const
{
	return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntity(Entity entity) 
{
	entities.push_back(entity);
}

void System::RemoveEntity(Entity entity) 
{

	//You could do a for loop however there is a better way
	//This will be using an iterator and an anynomous function
	//This is the most fucked way of going about a for loop function I've ever seen in my entire life
	
	//The advantages of using erase however is that it is compatible with any std container
	//Map, unordered map etc..
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) 
		{
			return entity == other;
		}
	),entities.end());

}

std::vector<Entity> System::GetSystemEntities() const 
{
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


		for (auto pool : componentPools)
		{
			if (pool)
			{
				pool->RemoveEntityFromPool(entity.GetId());
			}
		}
		
		freeIds.push_back(entity.GetId());
		RemoveEntityTag(entity);
		RemoveEntityGroup(entity);
		//Need to remove the entity from the component pools




	}
	entitiesToBeKilled.clear();
}

void Registry::TagEntity(Entity entity, const std::string& tag)
{
	//If it already exists in both then return
	if (entityPerTag.find(tag) != entityPerTag.end() && tagPerEntity.find(entity.GetId()) != tagPerEntity.end())
	{
		return;
	}

	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(entity.GetId(), tag);
}

Entity Registry::GetEntityByTag(const std::string& tag) const
{
	if (entityPerTag.find(tag) != entityPerTag.end())
	{
		return entityPerTag.at(tag);
	}
	return NULL;
}

void Registry::RemoveEntityTag(Entity entity)
{
	auto taggedEntity = tagPerEntity.find(entity.GetId());
	if (taggedEntity != tagPerEntity.end()) {
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}

void Registry::GroupEntity(Entity entity, const std::string& group)
{

	
	//No set exists
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
	{
		entitiesPerGroup.emplace(group, std::set<Entity>());
	}

	//a set does exist and the entity already has it
	if (entitiesPerGroup.at(group).find(entity) != entitiesPerGroup.at(group).end())
	{
		return;
	}

	//a set may have existed but not entity exists within the set and the other map
	entitiesPerGroup.at(group).emplace(entity);
	groupPerEntity.emplace(entity.GetId(), group);
	
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const
{
	if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
	{
		return false;
	}

	//make sure that both of them have them
	return entityPerTag.find(tag)->second == entity;
}
bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const
{
	//does not exist because no group exists
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
	{
		return false;
	}

	std::set<Entity> entities = entitiesPerGroup.at(group);

	//if it does not equal end then it does exist
	return entities.find(entity) != entities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const
{
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
	{
		//return empty vector
		return std::vector<Entity>();
	}
	std::set<Entity> entities = entitiesPerGroup.at(group);
	return std::vector<Entity>(entities.begin(), entities.end());
}

void Registry::RemoveEntityGroup(Entity entity)
{

	auto groupedEntity = groupPerEntity.find(entity.GetId());
	if (groupedEntity != groupPerEntity.end()) {
		auto group = entitiesPerGroup.find(groupedEntity->second);
		if (group != entitiesPerGroup.end()) {
			auto entityInGroup = group->second.find(entity);
			if (entityInGroup != group->second.end()) {
				group->second.erase(entityInGroup);
			}
		}
		groupPerEntity.erase(groupedEntity);
	}
}