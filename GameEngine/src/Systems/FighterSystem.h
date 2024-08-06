#pragma once
#include "../ECS/ECS.h"
#include "../Components/FAnimationComponent.h"
#include "../Components/InputBufferReceiverComponent.h"
class FighterSystem :public System
{
public:
	FighterSystem()
	{
		RequireComponent<FighterComponent>();
		RequireComponent<FAnimationComponent>();
	}
};