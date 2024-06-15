#pragma once

#include <string>

struct SpriteAnimationClip
{
	std::string animationClipId;
	std::map<float, int> timeToFrameVal;
	float duration;
	std::string spriteSheetId;

	//TODO: make map passed by move constructor

	SpriteAnimationClip(const std::string& animationStringId, const std::string& spriteSheetId, std::map<float, int> timeToFrameVal, float duration)
	{
		this->animationClipId = animationClipId;
		this->spriteSheetId = spriteSheetId;
		this->timeToFrameVal = timeToFrameVal;
		this->duration = duration;
	}

};
