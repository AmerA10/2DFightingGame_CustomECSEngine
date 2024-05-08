#pragma once

#include <glm/glm.hpp>
#include <string>
#include <SDL.h>
//This component is very flexible this is just an example created to beef up the engine examples I guess

/// <summary>
/// 
/// </summary>
/// <param name="vel"></param>
/// <param name="repeatFrequency"></param>
// <param name="projectileDuration"></param>
/// <param name="hitPercentDamage"></param>
/// <param name="isFriendly"></param>
struct ProjectileEmitterComponent 
{
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	bool isFriendly;
	int hitPercentDamage;
	int lastEmissionTime;
	std::string textureId;
	int projSpeed;
	bool useFrequency;



	ProjectileEmitterComponent(glm::vec2 vel = glm::vec2(0), int repeatFrequency = 0, int projectileDuration = 100, int hitPercentDamage = 10, bool isFriendly = true, int projSpeed = 0, bool useFrequency = true)
	{
		this->projectileVelocity = vel;
		this->repeatFrequency = repeatFrequency;
		this->hitPercentDamage = hitPercentDamage;
		this->projectileDuration = projectileDuration;
		this->isFriendly = isFriendly;
		this->lastEmissionTime = SDL_GetTicks();
		this->projSpeed = projSpeed;
		this->useFrequency = useFrequency;
	}

};