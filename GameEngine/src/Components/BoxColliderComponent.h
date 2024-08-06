#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Width: box width def = 0 
/// Height: box height def = 0
/// scale: width is multiplied by scale.x
/// scale: height is multiplied by scale.y
/// offset: Offset vec2 def = 0,0
/// </summary>
const unsigned int MAX_LAYERS = 32;
typedef std::bitset<MAX_LAYERS> Layer;
typedef std::bitset<MAX_LAYERS> Mask;

struct BoxColliderComponent {
	
	int width;
	int height;
	bool isCollliding;
	glm::vec2 offset;
	Layer layer;
	Mask mask;

public:
	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 scale = glm::vec2(1,1), glm::vec2 offset = glm::vec2(0), 
		const std::string& layer = "00000000000000000000000000000000", const std::string& mask = "00000000000000000000000000000000")
	{
		this->width = width * (int)scale.x;
		this->height = height * (int)scale.y;
		this->offset = offset;
		this->isCollliding = false;
		this->layer = Layer(layer);
		this->mask = Mask(mask);
	}



};