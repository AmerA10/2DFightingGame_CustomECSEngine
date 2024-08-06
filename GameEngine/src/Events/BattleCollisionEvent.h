#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include "../Components/BoxColliderComponent.h"

class BattleCollisionEvent : public Event
{
public:
	Entity a;
	Entity b;
	BoxColliderComponent boxA;
	BoxColliderComponent boxB;

	BattleCollisionEvent(Entity a, Entity b, BoxColliderComponent boxA, BoxColliderComponent boxB) : a(a), b(b), boxA(boxA), boxB(boxB)
	{

	}
};