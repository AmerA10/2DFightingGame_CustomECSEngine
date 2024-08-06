#pragma once

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include <SDL.h>
class ProjectileLifeCycleSystem: public System
{

public:
	ProjectileLifeCycleSystem()
	{
		RequireComponent<ProjectileComponent>();
	}

	void Update() 
	{
		for (auto entity : GetSystemEntities())
		{
			auto ProjectileComp = entity.GetComponent<ProjectileComponent>();

			if (SDL_GetTicks() - ProjectileComp.startTime > ProjectileComp.duration) 
			{
				entity.Kill();
			}

		}
	}

};