#pragma once
#include "../ECS/ECS.h"
#include "../Components/FAnimationComponent.h"
#include "../Components/InputBufferReceiverComponent.h"
#include "../Components/FighterComponent.h"
#include "../Components/BattleBoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
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
		RequireComponent<SpriteComponent>();
	}


	void Update(std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities())
		{
			FighterComponent& fighterComp = entity.GetComponent<FighterComponent>();
			InputBufferReceiverComponent& inputComp = entity.GetComponent<InputBufferReceiverComponent>();
			BattleBoxColliderComponent& boxComp = entity.GetComponent<BattleBoxColliderComponent>();
			FAnimationComponent& animComp = entity.GetComponent<FAnimationComponent>();
			SpriteComponent& spriteComp = entity.GetComponent<SpriteComponent>();

			InputAction currentRequestedAction = inputComp.GetCurrentAction();
			//Logger::Log("current requested action is: " + currentRequestedAction.inputActionName);

			//we can change our state
			if (fighterComp.currentMotion.canCancel || fighterComp.currentActionFrame >= fighterComp.currentActionFrameCount)
			{
				if (fighterComp.TryChangeMotion(currentRequestedAction))
				{
					animComp.PlayAnimClip(fighterComp.currentMotion.motionAnimClipId);
				}

				if (fighterComp.currentMotion.motionAction == "NONE")
				{
					fighterComp.currentState = FighterState::IDLE;
					fighterComp.currentActionVelocity = 0;
				}

				else if (fighterComp.currentMotion.motionAction == "MOVE_BCK")
				{
					fighterComp.currentState = FighterState::MOVING;
					fighterComp.currentActionVelocity = assetStore->GetFighter(fighterComp.fighterId)->backMoveSpeed;
					if (fighterComp.isFacingRight)
					{
						fighterComp.currentActionVelocity *= -1;
					}
				}

				else if (fighterComp.currentMotion.motionAction == "MOVE_FRWRD")
				{
					fighterComp.currentState = FighterState::MOVING;
					fighterComp.currentActionVelocity = assetStore->GetFighter(fighterComp.fighterId)->forwardMoveSpeed;
					if (!fighterComp.isFacingRight)
					{
						fighterComp.currentActionVelocity *= -1;

					}
				}

				else if (fighterComp.currentMotion.motionAction == "ATTACK")
				{
					fighterComp.currentState = FighterState::ATTACKING;
					fighterComp.currentActionVelocity = 0;
				}


			}
			boxComp.isFacingRight = fighterComp.isFacingRight;
			spriteComp.flip = fighterComp.isFacingRight == true ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			boxComp.InsertNewBoxes(fighterComp.GetCurrentFrameBoxes());
			fighterComp.currentActionFrame++;
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