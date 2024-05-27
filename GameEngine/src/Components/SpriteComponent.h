#pragma once
#pragma once

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>
/// <summary>
/// 
/// </summary>
/// <param name="assetId"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="zIndex"></param>
/// <param name="isFixed"></param>
/// <param name="srcRectX"></param>
/// <param name="srcRectY"></param>
struct SpriteComponent {

	int width;
	int height;
	int zIndex;
	std::string assetId;
	//This here essentially is a precursor to UI elements
	bool isFixed;
	SDL_Rect srcRect;
	SDL_RendererFlip flip;


	SpriteComponent(const std::string& assetId = "", int width = 20, int height = 20, int zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0) {
		this->width = width;
		this->height = height;
		this->assetId = assetId;
		this->zIndex = zIndex;
		this->isFixed = isFixed;
		this->srcRect = { srcRectX, srcRectY , this->width, this->height};
		this->flip = SDL_FLIP_NONE;

	}

	bool operator < (const SpriteComponent& other) const { return zIndex < other.zIndex; }

};

#endif