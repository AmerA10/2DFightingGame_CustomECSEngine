#pragma once

#include "../ECS/ECS.h"
#include "../Components/TestComponent.h"



class TestSystem : public System
{
	//We want to store our data in a contiguous memory so lets first do that

public:

	TestSystem()
	{

	}


	void Update()
	{

		return;
	}

};