#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Width: box width def = 0 
/// Height: box height def = 0
/// scale: width is multiplied by scale.x
/// scale: height is multiplied by scale.y
/// offset: Offset vec2 def = 0,0
/// </summary>
struct BoxColliderComponent {
	
	int width;
	int height;
	bool isCollliding;
	glm::vec2 offset;

public:
	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 scale = glm::vec2(1,1), glm::vec2 offset = glm::vec2(0)) {
		this->width = width * scale.x;
		this->height = height * scale.y;
		this->offset = offset;
		this->isCollliding = false;
	}



};