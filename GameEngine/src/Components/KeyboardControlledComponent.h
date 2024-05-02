#pragma once

#include <glm/glm.hpp>

struct KeyboardControlledComponent {

	glm::vec2 upVel;
	glm::vec2 rightVel;
	glm::vec2 downVel;
	glm::vec2 leftVel;


	KeyboardControlledComponent(glm::vec2 upVel = glm::vec2(0), glm::vec2 rightVel = glm::vec2(0), glm::vec2 downVel = glm::vec2(0), glm::vec2 leftVel = glm::vec2(0))
	{
		this->upVel = upVel;
		this->rightVel = rightVel;
		this->downVel = downVel;
		this->leftVel = leftVel;
	}

};