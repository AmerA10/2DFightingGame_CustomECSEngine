#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"

struct BattleBoxColliderComponent {

	std::vector<BoxColliderComponent> boxes;

	BattleBoxColliderComponent(std::vector<BoxColliderComponent> boxes = {})
	{
		this->boxes = boxes;
	}

	void AddHitBox(int width = 0, int height = 0, glm::vec2 scale = glm::vec2(1, 1), glm::vec2 offset = glm::vec2(0))
	{

	}

	void AddHurtBox(int width = 0, int height = 0, glm::vec2 scale = glm::vec2(1, 1), glm::vec2 offset = glm::vec2(0))
	{

	}
	void AddTouchBox(int width = 0, int height = 0, glm::vec2 scale = glm::vec2(1, 1), glm::vec2 offset = glm::vec2(0))
	{

	}

	void ClearBoxes()
	{
		boxes.clear();

	}




};