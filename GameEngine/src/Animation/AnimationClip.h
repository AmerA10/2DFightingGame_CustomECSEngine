#pragma once

#include <string>

struct AnimationClip
{
	float globalStartTime;
	float localTime;
	float playbackRate;
	float duration;
	int numLoops;

	std::string spriteSheetId;

	AnimationClip(const std::string& spriteSheetId, int playbackRate = 1, int duration = 1, int numLoops = 0)
	{
		this->duration = duration;
		this->playbackRate = playbackRate;
		this->numLoops = numLoops;
		this->localTime = 0;
		this->globalStartTime = 0;
		this->spriteSheetId = spriteSheetId;
	}

};

struct AnimationSpriteSheet
{
	std::string spriteSheetId;
	int spriteWidth;
	int spriteHeight;

	int numVCuts;
	int numHCuts;
	
	int sheetWidth;
	int sheetHeight;

	int xSrc;
	int ySrc;

	AnimationSpriteSheet(const std::string& assetId, int spriteWidth, int spriteHeight, int numVCuts, int numHCuts)
	{
		this->spriteSheetId = assetId;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;
		this->numVCuts = numVCuts;
		this->numHCuts = numHCuts;
	}


};