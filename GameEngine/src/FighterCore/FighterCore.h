#pragma once
#include <string>
#include <map>
struct FightMotion
{
	//need to know what motion this is
	std::string motionId;

	std::string motionAnimClipId;

	//what action activates this motion
	std::string motionAction;

	//how many frames does this motion last for?
	int motionDuration;

	bool canCancel;

	std::map<int, std::vector<BoxColliderComponent>> frameToBoxes;

	//need to know which animation corresponds to this motion


};

struct Fighter
{
	std::string id;
	std::vector<FightMotion> motions;
};

enum MotionType {
	IDLE = 0,
	MOVE = 1,
	ATTACK = 2,
	DAMAGE = 3
};