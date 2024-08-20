#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"

struct BattleBoxColliderComponent {

	std::vector<BoxColliderComponent> boxes;
	bool isFacingRight;

	BattleBoxColliderComponent(std::vector<BoxColliderComponent> boxes = {}, bool isFacingRight = true)
	{
		this->boxes = boxes;
		this->isFacingRight = isFacingRight;
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

	//this whole thing is extremely expensive for no reason
	void InsertNewBoxes(std::vector<BoxColliderComponent> newBoxes)
	{
		ClearBoxes();
		this->boxes.insert(boxes.end(), newBoxes.begin(), newBoxes.end());
	}

	void ClearBoxes()
	{
		this->boxes.clear();

	}


};