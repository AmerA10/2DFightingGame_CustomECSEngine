#pragma once

#include "../ECS/ECS.h"

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class CameraMovementSystem : public System
{
public:
	CameraMovementSystem() 
	{
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(SDL_Rect& camera) 
	{
		for (auto entity : GetSystemEntities()) 
		{

			//TODO: Can create an offset and boundaries variales that a part of the system or the component that can be used
			//to define different offsets and boundaries instead of half the screen

			const TransformComponent& transform = entity.GetComponent<TransformComponent>();


			if (transform.position.x + (camera.w / 2)< Game::mapWidth)
			{
				camera.x = transform.position.x - (Game::windowWidth/2);
			}
			if (transform.position.y + (camera.h/2)< Game::mapHeight)
			{
				camera.y = transform.position.y - (Game::windowHeight / 2);
			}

			//TODO: keep the camera rec view inside the screen limits
			camera.x = glm::max(0, camera.x);
			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = glm::max(0, camera.y);
			camera.y = camera.y > camera.h ? camera.h : camera.y;



		}
	}

};