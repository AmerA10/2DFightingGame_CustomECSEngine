#pragma once

#include "../ECS/ECS.h"
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/RigidBodyComponent.h"
#include <glm/glm.hpp>


class RenderGUISystem : public System
{
public:

	//This system is not really interested in current entities in the scenes
	RenderGUISystem() = default;

	void Update(const std::unique_ptr<Registry>& registry)
	{
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		//ImGui has window flags, we can use the '|' to do a bit wise join of the flags

		//because we are in immediate mode, if it was not a static, we are always recreating the values
		//We need to persist the values as such it must be static

		//Create new window
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
		if (ImGui::Begin("Spawn Enemies"), windowFlags)
		{

			static int pos[2] = { 0,0 };
			static int vel[2] = { 0,0 };
			static int scale = 1;
			static int rotation = 0;
			static int healthPercentage = 100;
			static int projVel[2] = { 0,0 };

			ImGui::Text("Creating Spawn Enemies Window");
			//TODO: Create button to spawn enemies , we need an input for x and Y position,
			ImGui::InputInt2("Enemy Position", pos);
			ImGui::InputInt2("Enemy Velocity", vel);
			ImGui::InputInt("Scale", &scale);
			ImGui::InputInt("Rotation", &rotation);

			ImGui::DragInt("Health Percentage", &healthPercentage, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

		
			ImGui::InputInt2("Projectile Velocity", projVel);

			const char* itemsSprite[] = { "tank-texture" , "truck-texture" };
			static int currentSpriteItemIndex = 0;

			if (ImGui::BeginCombo("Enemy Sprite", itemsSprite[currentSpriteItemIndex]))
			{
				for (int i = 0; i < IM_ARRAYSIZE(itemsSprite); i++)
				{
					const bool isSelected = currentSpriteItemIndex == i;
					if (ImGui::Selectable(itemsSprite[i], isSelected))
					{
						currentSpriteItemIndex = i;

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndCombo();
			}

			const char* itemsFont[] = {"charriot-font" , "arial-font"};
			static int currentFontItemIndex = 0;

			if (ImGui::BeginCombo("Text Label Font", itemsFont[currentFontItemIndex]))
			{
				for (int i = 0; i < IM_ARRAYSIZE(itemsFont); i++)
				{
					const bool isSelected = currentFontItemIndex == i;
					if (ImGui::Selectable(itemsFont[i], isSelected))
					{
						currentFontItemIndex = i;

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Spawn Enemy"))
			{
				Entity enemy = registry->CreateEntity();
				enemy.AddComponent<TransformComponent>(glm::vec2(pos[0], pos[1]), glm::vec2(scale, scale), rotation);
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(vel[0], vel[1]));
				enemy.AddComponent<SpriteComponent>(itemsSprite[currentSpriteItemIndex], 32, 32, scale);
				enemy.AddComponent<BoxColliderComponent>(32, 32, enemy.GetComponent<TransformComponent>().scale, glm::vec2(0,0));
				enemy.Group("Enemies");
				enemy.AddComponent<HealthComponent>(healthPercentage);
				SDL_Color startColor = { 10,255,10 };
				enemy.AddComponent<TextLabelComponent>(itemsFont[currentFontItemIndex], std::to_string(enemy.GetComponent<HealthComponent>().health), enemy.GetComponent<TransformComponent>().position, startColor, false);
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVel[0], projVel[1]), 1000, 7000, 10, false, 80, true);
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	}


};