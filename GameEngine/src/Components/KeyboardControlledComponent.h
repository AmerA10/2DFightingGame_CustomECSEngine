#pragma once

#include <glm/glm.hpp>

struct KeyboardControlledComponent {

	glm::vec2 yVel;
	glm::vec2 xVel;



	KeyboardControlledComponent(glm::vec2 upVel = glm::vec2(0), glm::vec2 rightVel = glm::vec2(0))
	{
		this->yVel = upVel;
		this->xVel = rightVel;

	}

};