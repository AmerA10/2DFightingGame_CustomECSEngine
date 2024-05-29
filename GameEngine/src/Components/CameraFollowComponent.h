#pragma once
#include "../ECS/ECS.h"

struct CameraFollowComponent 
{

	bool follow;

	CameraFollowComponent(bool follow = true) {
		this->follow = follow;
	}
		
};