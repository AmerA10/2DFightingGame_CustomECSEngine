#pragma once

#include "../ECS/ECS.h"
#include <map>
#include "../Components/TestComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"



class TestSystem : public System
{
	//We want to store our data in a contiguous memory so lets first do that

private:

	int size;

public:
	TestSystem()
	{

	}




	void Update()
	{


	}

};