#pragma once


#include<SDL.h>

// we need to keep track of our animation's current state
//as an will have more than one animtion to play

enum AnimationState
{
	Stopped, Playing, WaitingToPlay
};

struct AnimationComponent {
	float globalStartTime;
	float localTime;
	float playbackRate;
	//this is in frames or seconds?? lets say frames
	float duration;
	int numLoops;

	std::string spriteSheetId;
	std::string animationId;
	AnimationState animState;

	AnimationComponent(const std::string& spriteSheetId, int playbackRate = 1, float duration = 1.0, int numLoops = 0) {
		this->duration = duration;
		this->playbackRate = playbackRate;
		this->numLoops = numLoops;
		this->localTime = 0;
		this->globalStartTime = 0;
		this->spriteSheetId = spriteSheetId;
		animState = WaitingToPlay;
	}
};

