#pragma once
#ifndef ECS_H

#define ECS_H

#include <bitset>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <set>
#include <deque>
#include "../logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

/// <summary>
/// We use a bitset (1s and 0s) to track of which components an entity has 
/// also helps keep track of which entities a system is interested in
/// </summary>
typedef std::bitset<MAX_COMPONENTS> Signature;

//We want a component ID per component type
struct IComponent {
protected:
	static int nextId;

};

//We have a template class, used to assign a unique Id to a component type
//We want to return a unique per type T

//Remember that static variables are only initialized once so 
// static auto id = nextID will only run the first GetId gets called
//unique id per class
template<typename T>
class Component: public IComponent {
public:
	static int GetId() {
		//Post increment
		static auto id = nextId++;
		return id;
	}

};

//Forward declare the registry class
class Registry;

class Entity {
private:
	int id;

public:
	//method will not modify the state of the class, intiallize member id
	Entity(int id) : id(id) {}
	int GetId() const;

	//This constructor will use the overloaded assignmenet operator
	Entity(const Entity& entity) =default;

	void Kill();

	//Default assignment overloading operator
	Entity& operator =(const Entity& other) = default;
	//Operator overloading 
	bool operator == (const Entity& other) const {return id == other.id;}
	bool operator != (const Entity& other) const {return id != other.id;}
	bool operator > (const Entity& other) const {return id > other.id;}
	bool operator < (const Entity& other) const {return id < other.id; }

	//Hold a pointer to the entity's owner registry
	//can't make this a weak pointer I don't think, actually it probably should be a weak pointer
	//This cretes a cyclical dependency so Idk why the prof decided to do this tbh
	//There is probabl a better way of handing this, an interface or something
	//This pointer is also not deleted when this entity is deleted 
	Registry* registry;

	template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template<typename TComponent> void RemoveComponent();
	template<typename TComponent> bool HasComponent() const;
	template<typename TComponent> TComponent& GetComponent() const;

	//Manage entity tags and Groups
	void Tag(const std::string& tag);
	bool HasTag(const std::string& tag)const;
	void Group(const std::string& group);
	bool BelongsToGroup(const std::string& group)const;

};

/// <summary>
/// System processes entities that contain a specific signature
/// </summary>
class System {
private:
		Signature componentSignature;
		std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);

	std::vector<Entity> GetSystemEntities()const;
	const Signature& GetComponentSignature()const;

	//Defines the components type that entities must have to be considered by the system
	template <typename TComponent> void RequireComponent();

};

class IPool {
public:
	//By forcing the destructor to be pure virtual, it means that no type
	//IPool object can be instantiated
	//Example: Ipool poolObj; will give a compilerError
	//This is not pure virtual but it can be
	virtual ~IPool() {};

	virtual void RemoveEntityFromPool(int entityId) = 0;
};

/// <summary>
/// A pool is just a vector (contiguous data) of objects of Type T
/// </summary>
template <typename T>
class Pool: public IPool {
	private:
		std::vector<T> data;
		int size;

		std::unordered_map<int, int> entityIdToIndex;
		std::unordered_map<int, int> indexToEntityId;

	public:
		Pool(int capacity = 100) {
			size = 0;
			data.resize(capacity);
		}

		 ~Pool() = default;

		bool IsEmpty() {
			return size == 0;
		}
		int GetSize() {
			return size;
		}
		void Resize(int n) {
			data.resize(n);
		}
		void Clear() {
			data.clear();
			size = 0;
			entityIdToIndex.clear();
			indexToEntityId.clear();
		}

		void Add(T object) {
			data.push_back(object);
		}

		void Set(int entityId, T object) {

			//the entity already has an object in this pool so we just have to replace it
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
			{
				int index = entityIdToIndex[entityId];
				data[index] = object;
			}

			else 
			{
				int index = size;
				entityIdToIndex.emplace(entityId, index);
				indexToEntityId.emplace(index, entityId);

				if (size >= data.capacity())
				{
					data.resize(size * 2);
				}
				data[index] = object;
				size++;

			}
		}

		void Remove(int entityId)
		{
			//no such entityId to remove just get out then
			//if (entityIdToIndex.find(entityId) == entityIdToIndex.end())
			//{
			//	return;
			//}

			int indexOfRemoved = entityIdToIndex[entityId];
			int indexOfLast = size - 1;

			//Switched removed with the last
			//Ok but what if our removed is our last? that should be dealt with to be honest
			data[indexOfRemoved] = data[indexOfLast];

			//update the index-entity mapps to point to the correct elements
			int entityIdOfLastElement = indexToEntityId[indexOfLast];
			entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
			indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

			entityIdToIndex.erase(entityId);
			indexToEntityId.erase(indexOfLast);

			size--;

		}

		void RemoveEntityFromPool(int entityId) override
		{
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
			{
				Remove(entityId);
			}
		
		}

		T& Get(int entityId) {
			//To make sure we return the correct datatype
			int index = entityIdToIndex[entityId];
			return static_cast<T&>(data[index]);
		}

		T& operator [] (unsigned int index) {
			return data[index];
		}

		std::unordered_map<int,int> GetEntityIdToIndexMap()
		{
			return entityIdToIndex;
		}
};

/// <summary>
/// Registry Class, responsible for creation and destruction of entities,Systems, and components
/// It is also responsible for housing the components and entities and systems
/// </summary>
class Registry {

	private:
		int numEntities = 0;
		
		//Set of entities that are flagged to be added or removed
		//In the next registry update
		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeKilled;
	
		/// <summary>
		/// Vector component of pools, each pool contains all the data for a certain component
		/// Where each vector index is the component type id
		/// and the pool index is the entity id
		/// This connects the entities to the components
		/// </summary>
		std::vector<std::shared_ptr<IPool>> componentPools;
		/// <summary>
		/// Vector compnent of signatures
		/// the signatures let us know which components are turned "on" for an entity
		/// where the vector index = entity id
		/// </summary>
		std::vector<Signature> entityComponentSignatures;

		/// <summary>
		/// a map of keys and values that is not ordered in memory
		/// </summary>
		std::unordered_map<std::type_index, std::shared_ptr<System>> systemsMap;


		//This is a double ended que used to reuse the ids of entities that were previously removed
		std::deque<int> freeIds;

		//Maps of Entity Tags, one gives the entity from the tag
		//the other gives the tag from the entity ID
		std::unordered_map<std::string, Entity> entityPerTag;
		std::unordered_map<int, std::string> tagPerEntity;

		//Maps of Group 
		std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
		std::unordered_map<int, std::string> groupPerEntity;


	public:

		Registry() {
			Logger::Log("Registry Constructor Called");
		}
		~Registry()
		{
			Logger::Log("Registry Destructor Called");
		}
		//TODO:

		void Update();

		//Entity Management
		//
		
		Entity CreateEntity();
		//KilEntity();
		void KillEntity(Entity entity);

		//Component Management
		//
		//We only add an entity to a given system if the component signature 
		//of the system and the entity matches
		template<typename TComponent, typename ...TArgs>
		void AddComponent(Entity entity, TArgs&& ... args);
		
		template<typename TComponent>
		void RemoveComponent(Entity entity);
		
		template<typename TComponent>
		bool HasComponent(Entity entity) const;

		template<typename TComponent>
		TComponent& GetComponent(Entity entity) const;

		//System Management
		//
		template<typename TSystem, typename ...TArgs>
		void AddSystem(TArgs&& ...args);

		template<typename TSystem>
		void RemoveSystem();

		template<typename TSystem>
		bool HasSystem() const;
		
		template <typename TSystem>
		TSystem& GetSystem() const;

		//check the component signature of the entity
		//add the entity to the systems that are interested in it
		void AddEntityToSystems(Entity entity);

		void RemoveEntityFromSystems(Entity entity);

		//Tag Functions
		void TagEntity(Entity entity, const std::string& tag);
		bool EntityHasTag(Entity entity, const std::string& tag)const;
		Entity GetEntityByTag(const std::string& tag) const;
		void RemoveEntityTag(Entity entity);

		//Group Functions
		void GroupEntity(Entity entity, const std::string& group);
		bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
		std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
		void RemoveEntityGroup(Entity entity);

		//Pool functions
		template<typename TComponent>
		std::shared_ptr<Pool<TComponent>> GetComponentPool() const;
		template<typename TComponent>
		std::unordered_map<int, int> GetEntityIdIndexMapForComponent()const ;
};

template<typename TComponent>
std::shared_ptr<Pool<TComponent>> Registry::GetComponentPool() const
{

	int componentId = Component<TComponent>::GetId();
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool;
}

template<typename TComponent>
std::unordered_map<int, int> Registry::GetEntityIdIndexMapForComponent() const
{
	return GetComponentPool<TComponent>()->GetEntityIdToIndexMap();
}


///////Template Function Implementations REGISTRY


template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
	//Create the system we want to add fowrwarding the parameters 
	//Same thing as saying Tsystem* newwSystem = new TSystem(std::forward<TArgs>(args)...);
	
	//We are creating a new system and inserting it into a new map 
	//so this is probably not a unique pointer as multiple different
	//places will probably need it
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);

	//Create a new key pair value and add it to the 
	//There is also other syntaxes I can use but now this works
	systemsMap.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem() {

	auto system = systemsMap.find(std::type_index(typeid(TSystem)));

	systemsMap.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {

	//if the pointer returned is different from the end pointer then we have found the system
	return systemsMap.find(std::type_index(typeid(TSystem))) != systemsMap.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const {

	auto system = systemsMap.find(std::type_index(typeid(TSystem)));
	//the map returns a system* but we want a TSystem then we want to dereference it from a pointer then returned as a reference
	return *(std::static_pointer_cast<TSystem>(system->second));
}

//Registry Templates
template<typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	//need to resize the component pool if the id of the component is bigger than the size
	//As the componentIds increase by one for each unique component type
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	//if we do not have a pool for the component type
	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>(Pool<TComponent>());

		//we could potentially do a Move semantics thing here but im not sure
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	//Not needed anymore
	//if (entityId >= componentPool->GetSize()) {
	//	componentPool->Resize(numEntities);
	//}

	//Forward the multiple param
	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	entityComponentSignatures[entityId].set(componentId);

	Logger::Log("Adding Component - Component ID = " + std::to_string(componentId) + " added to the Entity with ID: " + std::to_string(entityId));

	Logger::Log("New Pool size: " + std::to_string(componentPool->GetSize()));
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	//Can do this but
	//return entityComponentSignatures[entityId][componentId] == 1 ? true : false;

	//This function already returns true or false
	return entityComponentSignatures[entityId].test(componentId);

}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);


	//We do need to remove the it from the pool 
	componentPool->Remove(entityId);

	//Set component signature ID to false
	entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	
	return componentPool->Get(entityId);

}


//Template Function Implementations ENTITY


template<typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
	registry->AddComponent<TComponent>(*this, std::forward<TArgs&&>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}

//Template Function Implementations SYSTEM

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
	Logger::Log("System Needs component signature of :" + componentSignature.to_string());

}

#endif