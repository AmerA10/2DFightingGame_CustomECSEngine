#pragma once

#include <string>

#include <sol/sol.hpp>

struct SpriteAnimationClip
{
	std::string animationClipId;
	std::map<float, int> timeToFrameVal;
	std::map<float, sol::function> timeToEvent;
	float duration;
	std::string spriteSheetId;

	std::vector<sol::function> calledFuncs;
	//TODO: make map passed by move constructor

	SpriteAnimationClip(const std::string& animationStringId, const std::string& spriteSheetId, std::map<float, int> timeToFrameVal, float duration, std::map<float, sol::function> timeToEvent)
	{
		this->animationClipId = animationClipId;
		this->spriteSheetId = spriteSheetId;
		this->timeToFrameVal = timeToFrameVal;
		this->duration = duration;
		this->timeToEvent = timeToEvent;
	}



};

