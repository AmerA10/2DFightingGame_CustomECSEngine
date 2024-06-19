#pragma once


// we need to keep track of our animation's current state
//as an will have more than one animtion to play


#include <sol/sol.hpp>

enum AnimationState
{
	Stopped, Playing, WaitingToPlay
};
struct AnimationComponent {

	std::string animClipId;
	float playbackRate;
	int numLoops;

	//this is in frames or seconds?? lets say frames
	float globalStartTime;
	float localTime;
	AnimationState animState;

	std::vector<sol::function> calledFuncs;

	int frameVal;

	AnimationComponent(const std::string& animClipId = "", float playbackRate = 1.0f, int numLoops = 1 )
	{
		this->animClipId = animClipId;
		this->playbackRate = playbackRate;
		this->numLoops = numLoops;
		this->globalStartTime = 0.0f;
		this->localTime = 0.0f;
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
};

