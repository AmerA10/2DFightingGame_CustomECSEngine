#pragma once

#include "../ECS/ECS.h"
#include "../Components/HealthComponent.h";
#include "../Components/TransformComponent.h"
#include "../Components/TextLabelComponent.h"

class HealthDisplaySystem : public System
{
public:
	HealthDisplaySystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<TextLabelComponent>();
		RequireComponent<HealthComponent>();
	}

	void Update(SDL_Renderer* renderer, const SDL_Rect& camera)
	{
		for (Entity& entity: GetSystemEntities())
		{
			TextLabelComponent& healthLabel = entity.GetComponent<TextLabelComponent>();
			const HealthComponent& healthData = entity.GetComponent<HealthComponent>();
			const TransformComponent& transformComp = entity.GetComponent<TransformComponent>();

			healthLabel.text = std::to_string(healthData.health) + "%";

			healthLabel.position = transformComp.position + glm::vec2(70, 0);

			if (healthData.health >= 66)
			{
				healthLabel.color = { 0,255,50 };
			}
			else if (healthData.health < 66 && healthData.health >= 33)
			{
				healthLabel.color = { 200,150,0 };
			}
			else
			{
				healthLabel.color = { 255,50,0 };
			}


			int barWidth = 40;
			int barHeight = 10;

			SDL_Rect barRect =
			{
				static_cast<int>(transformComp.position.x + 70 - camera.x),
				static_cast<int>(transformComp.position.y + 20 + barHeight - camera.y),
				static_cast<int>(barWidth * (healthData.health / 100.0)),
				static_cast<int>(barHeight)
			};

			SDL_SetRenderDrawColor(renderer, healthLabel.color.r, healthLabel.color.g, healthLabel.color.b,255);
			
			SDL_RenderFillRect(renderer, &barRect);

			SDL_RenderDrawRect(renderer, &barRect);

		}
	}

};