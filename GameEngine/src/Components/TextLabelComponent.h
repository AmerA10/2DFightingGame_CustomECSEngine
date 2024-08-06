#pragma once

#include <glm/glm.hpp>
#include <string>
#include <SDL.h>

/// <summary>
/// 
/// </summary>
/// <param name="assetId"></param>
/// <param name="text"></param>
/// <param name="vec2"></param>
/// <param name="color"></param>
/// <param name="isFixed"></param>
struct TextLabelComponent
{
	std::string assetId;
	std::string text;
	glm::vec2 position;
	SDL_Color color;
	bool isFixed;


	TextLabelComponent(const std::string assetId = "", const std::string & text = "", glm::vec2 position = glm::vec2(0), const SDL_Color & color = {0,0,0},  bool isFixed = true)
	{
		this->assetId = assetId;
		this->text = text;
		this->position = position;
		this->color = color;
		this->isFixed = isFixed;
	}

};