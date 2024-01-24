#pragma once

#include <glm/glm.hpp>

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

struct TransformComponent {

	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

};

#endif