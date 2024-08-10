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
			FAnimationComponent& animComp = entity.GetComponent<FAnimationComponent>();

			InputAction currentRequestedAction = inputComp.GetCurrentAction();
			//Logger::Log("current requested action is: " + currentRequestedAction.inputActionName);

			//we can change our state
			if (fighterComp.currentMotion.canCancel || fighterComp.currentActionFrame >= fighterComp.currentActionFrameCount)
			{
				if (fighterComp.TryChangeMotion(currentRequestedAction.inputActionName))
				{
					animComp.PlayAnimClip(fighterComp.currentMotion.motionAnimClipId);
					Logger::Log("Changing action to: " + currentRequestedAction.inputActionName);
				}

				if (currentRequestedAction.inputActionName == "NONE")
				{
					fighterComp.currentState = FighterState::IDLE;
				
				}
				if (currentRequestedAction.inputActionName == "MOVEY")
				{
					
					fighterComp.currentState = FighterState::MOVING;

				}
				if (currentRequestedAction.inputActionName == "ATTACK")
				{
					fighterComp.currentState = FighterState::ATTACKING;

				}


			}

			fighterComp.currentActionFrame++;
			//need to update boxes
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