#pragma once
#include "../ECS/ECS.h"
#include "../Components/FAnimationComponent.h"
#include "../Components/InputBufferReceiverComponent.h"
#include "../Components/FighterComponent.h"
#include "../Components/BattleBoxColliderComponent.h"
#include "../AssetStore/AssetStore.h"
class FighterSystem :public System
{
public:
	FighterSystem()
	{
		RequireComponent<FighterComponent>();
		RequireComponent<FAnimationComponent>();
		RequireComponent<InputBufferReceiverComponent>();
		RequireComponent<BattleBoxColliderComponent>();
	}


	void Update()
	{
		for (auto entity : GetSystemEntities())
		{
			FighterComponent& fighterComp = entity.GetComponent<FighterComponent>();
			InputBufferReceiverComponent& inputComp = entity.GetComponent<InputBufferReceiverComponent>();
			BattleBoxColliderComponent& boxComp = entity.GetComponent<BattleBoxColliderComponent>();

			InputAction currentRequestedAction = inputComp.GetCurrentAction();
			//Logger::Log("current requested action is: " + currentRequestedAction.inputActionName);
			if (currentRequestedAction.inputActionName == "NONE")
			{
			//	Logger::Log("not doing anything");
			}
			if (currentRequestedAction.inputActionName == "MOVEY")
			{
			//	Logger::Log("Trying to move");

			}
			if (currentRequestedAction.inputActionName == "ATTACK")
			{
			//	Logger::Log("Trying to attack");

			}


		}
	}

	void SetUpFighters(std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities())
		{
			FighterComponent& fighterComp = entity.GetComponent<FighterComponent>();

			std::unique_ptr<Fighter>& fighterData = assetStore->GetFighter(fighterComp.fighterId);
			for (auto motion : fighterData->motions)
			{
				
				fighterComp.SetupMotion(assetStore->GetFightMotion(motion));

			}
			
		}
	}
};