#pragma once
#pragma once

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>

struct SpriteComponent {

	int width;
	int height;
	std::string assetId;

	SDL_Rect srcRect;

	SpriteComponent(const std::string& assetId = "", int width = 20, int height = 20, int srcRectX = 0, int srcRectY = 0) {
		this->width = width;
		this->height = height;
		this->assetId = assetId;

		this->srcRect = { srcRectX, srcRectY , this->width, this->height};

	}

};

#endif