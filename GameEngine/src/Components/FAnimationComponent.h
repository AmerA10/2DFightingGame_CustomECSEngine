#pragma once
#pragma once


// we need to keep track of our animation's current state
//as an will have more than one animtion to play


#include <sol/sol.hpp>
#include "../Animation/AnimationClip.h"

struct FAnimationComponent {

	std::string animClipId;
	int playbackRate;

	//this is in frames or seconds?? lets say frames
	float globalStartTime;
	int localFrame;
	AnimationState animState;

	std::vector<sol::function> calledFuncs;

	int frameVal;

	FAnimationComponent(const std::string& animClipId = "", int playbackRate = 1)
	{
		this->animClipId = animClipId;
		this->playbackRate = playbackRate;
		this->globalStartTime = 0.0f;
		this->localFrame = 0;
		this->animState = WaitingToPlay;
		this->frameVal = 0;
	}

	void PlayAnimClip(const std::string& animClipId)
	{
		this->animClipId = animClipId;
		this->playbackRate = 1;
		this->localFrame = 0;
		this->animState = WaitingToPlay;
		this->frameVal = 0;

	}

	bool HasPlayedClip(sol::function func)
	{
		for (sol::function f : calledFuncs)
		{
			if (func == f)
			{
				return true;
			}
		}

		return false;
	}

	~FAnimationComponent() = default;
};

