#pragma once
#ifndef ECS_H

#define ECS_H

#include <bitset>;
#include <vector>;
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
	inline static int count = 0;

	static int GetCount() {
		static int currentCount = count++;
		return currentCount;
	}

	static int GetId() {
		//Post increment
		Logger::Log("Calling get id on:");
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

class Registry {

};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif