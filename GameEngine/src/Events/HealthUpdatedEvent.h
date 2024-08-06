#pragma once
#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class HealthUpdatedEvent : public Event
{
public:
	int updatedHealth;
	Entity a;

	HealthUpdatedEvent(Entity a, int updatedHealth) : a(a), updatedHealth(updatedHealth)
	{

	}
};