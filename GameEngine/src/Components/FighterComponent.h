#pragma once
#include <string>
#include <map>
#include "../FighterCore/FighterCore.h"
#include "../Components/BoxColliderComponent.h"
struct FighterComponent
{
	std::string figther_id;
	std::string currentMotionId;

	//Where are we in the current motion that we are doing
	int currentActionFrame;

	//How many frames are there in this action?
	int currentActionFrameCount;

	int currentActionVelocity;

	FightMotion currentMotion;

	std::unordered_map<std::string, FightMotion> motions;

	//need to store a way to access the actions

};

