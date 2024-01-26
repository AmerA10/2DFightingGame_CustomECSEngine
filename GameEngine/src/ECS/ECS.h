#pragma once
#ifndef ECS_H

#define ECS_H

#include <bitset>;
#include <vector>;
#include <unordered_map>;
#include <typeindex>;
#include <typeinfo>;
#include <set>;
#include "../logger/Logger.h";

const unsigned int MAX_COMPONENTS = 32;

/// <summary>
/// We use a bitset (1s and 0s) to track of which compoentsn an entity has 
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

class Entity {
private:
	int id;

public:
	//method will not modify the state of the class, intiallize member id
	Entity(int id) : id(id) {};
	int GetId() const;

	//This constructor will use the overloaded assignmenet operator
	Entity(const Entity& entity) =default;

	//Default assignment overloading operator
	Entity& operator =(const Entity& other) = default;
	//Operator overloading 
	bool operator == (const Entity& other) const {return id == other.id;}
	bool operator != (const Entity& other) const {return id != other.id;}
	bool operator > (const Entity& other) const {return id > other.id;}
	bool operator < (const Entity& other) const {return id < other.id; }


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
	virtual ~IPool() = 0;
};

/// <summary>
/// A pool is just a vector (contiguous data) of objects of Type T
/// </summary>
template <typename T>
class Pool: public IPool {
	private:
		std::vector<T> data;

	public:
		Pool(int size = 100) {
			data.resize(size);
		}

		virtual ~Pool() = default;

		bool isEmpty() {
			return data.empty();
		}
		int GetSize() {
			return data.size();
		}
		void Resize(int n) {
			data.resize(n);
		}
		void Clear() {
			data.clear();
		}

		void Add(T object) {
			data.push_back(object);
		}
		void Set(int index, T object) {
			data[index] = object;
		}

		T& Get(int index) {
			//To make sure we return the correct datatype
			return static_cast<T&>(data[index]);
		}

		T& operator [] (unsigned int index) {
			return data[index];
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
		/// </summary>
		std::vector<IPool*> componentPools;
		
		/// <summary>
		/// Vector compnent of signatures
		/// the signatures let us know which components are turned "on" for an entity
		/// where the vector index = entity id
		/// </summary>
		std::vector<Signature> entityComponentSignatures;

		/// <summary>
		/// a map of keys and values that is not ordered in memory
		/// </summary>
		std::unordered_map<std::type_index, System*> Systems;

	public:
		//TODO:

		void Update();

		Entity CreateEntity();

		//We only add an entity to a given system if the component signature 
		//of the system and the entity matches 
		void AddEntityToSystem(Entity entity);
		//KilEntity();
		//
		//AddComponent(Entity entity);
		//RemoveComponent(Entity entity);
		//HasComponent(Entity entity);
		//
		//AddSystem();
		//RemoveSystem();
		//HasSystem();
		//GetSystem();

};



template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif