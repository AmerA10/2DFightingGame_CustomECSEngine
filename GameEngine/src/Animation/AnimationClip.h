#pragma once

#include <string>

#include <sol/sol.hpp>
#include <glm/glm.hpp>

enum AnimationState
{
	Stopped, Playing, WaitingToPlay
};

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

struct FAnimationClip
{
	std::string clipId;
	std::map<int, int> frameNumToSheetIndex;
	std::map<int, sol::function> frameNumToEvent;
	int duration;
	std::string spriteSheetId;
	std::vector<sol::function> calledFuncs;


	FAnimationClip(const std::string& animationStringId, const std::string& spriteSheetId, std::map<int, int> frameNumToSheetIndex, int duration, std::map<int, sol::function> frameNumToEvent)
	{
		this->clipId = clipId;
		this->spriteSheetId = spriteSheetId;
		this->frameNumToSheetIndex = frameNumToSheetIndex;
		this->duration = duration;
		this->frameNumToEvent = frameNumToEvent;

	}

};

struct SpriteSheet
{
	std::string sheetId;
	int numFrames;
	std::map<int, SDL_Rect> indexToRect;

};