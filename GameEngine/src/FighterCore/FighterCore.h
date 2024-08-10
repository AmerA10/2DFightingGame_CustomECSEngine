#pragma once
#include <string>
#include <map>
#include "../Components/BoxColliderComponent.h"

enum MotionType {
	NONE = 0,
	MOVE = 1,
	ATTACK = 2,
	DAMAGE = 3
};

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
	MotionType motionType;

public:
	FightMotion(const FightMotion& motion)
	{
		this->motionId = motion.motionId;
		this->motionAnimClipId = motion.motionAnimClipId;
		this->motionDuration = motion.motionDuration;
		this->canCancel = motion.canCancel;
		this->frameToBoxes = motion.frameToBoxes;
		this->motionType = motion.motionType;
	}
	FightMotion() = default;

};

struct Fighter
{
	std::string id;
	std::vector<std::string> motions;

	int backMoveSpeed;
	int forwardMoveSpeed;
};

