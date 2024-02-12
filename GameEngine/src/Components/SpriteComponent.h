#pragma once
#pragma once

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>

struct SpriteComponent {

	int width;
	int height;
	std::string assetId;

	SpriteComponent(const std::string& assetId = "", int width = 20, int height = 20) {
		this->width = width;
		this->height = height;
		this->assetId = assetId;
	}

};

#endif