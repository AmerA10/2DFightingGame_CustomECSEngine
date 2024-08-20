#pragma once
#include <string>
#include <map>
#include "../FighterCore/FighterCore.h"
#include "../Components/BoxColliderComponent.h"
#include <memory>

enum FighterState
{
	IDLE = 0, MOVING = 1, ATTACKING = 2, HURT = 3
};

struct FighterComponent
{
	std::string fighterId;

	//Where are we in the current motion that we are doing
	int currentActionFrame;

	//How many frames are there in this action?
	int currentActionFrameCount;

	int currentActionVelocity;

	FightMotion currentMotion;
	FighterState currentState;

	//action to motion map
	std::unordered_map<std::string, FightMotion> actionToMotions;

	bool isFacingRight;

public:
	FighterComponent(const std::string& fighterId = "", bool isFacingRight = true)
	{
		this->fighterId = fighterId;
		this->currentActionFrame = 0;
		this->currentActionFrameCount = 0;
		this->currentActionVelocity = 0;
		this->currentMotion = {};
		this->isFacingRight = isFacingRight;
		currentState = FighterState::IDLE;
	}

	//how do we change our motion? what is the driving mechanis that changes our actions that we are doing
	//Well two things, input, other events from other actions
	bool TryChangeMotion(InputAction& newAction)
	{

		std::string& newActionName = newAction.inputActionName;
		//Convert the MOVEY to MOVE_BCK or MOVE_FRWRD 
		if (newActionName == "MOVEY")
		{
			// we want to move left
			if (newAction.scale > 0)
			{
				//move back
				if (isFacingRight)
				{

					newActionName = "MOVE_BCK";
				}
				else
				{
					newActionName = "MOVE_FRWRD";
				}
			}
			//want to move right
			else
			{
				//move forward
				if (isFacingRight)
				{
					newActionName = "MOVE_FRWRD";
				}
				else
				{
					newActionName = "MOVE_BCK";
				}
			}
		}

		if (actionToMotions.find(newActionName) == actionToMotions.end())
		{
			return false;
		}


		const FightMotion& newMotion = actionToMotions.at(newActionName);
		if (currentMotion.motionId == newMotion.motionId && currentActionFrame < currentActionFrameCount)
		{
			return false;
		}

		
		currentMotion = newMotion;
		currentActionFrameCount = currentMotion.motionDuration;
		currentActionFrame = 0;

		
		return true;

	}

	void TryTakingDamage()
	{

	}

	void SetupMotion(std::unique_ptr<FightMotion>& motionToAdd)
	{
		
		//I can do this instead since I have created a copy constructor but that is horrible practice
		this->actionToMotions.emplace(motionToAdd->motionAction, FightMotion(*motionToAdd.get()));

		
		//this->actionToMotions.emplace(motionToAdd->motionAction, motionToAdd->motionId);
	}

	std::vector<BoxColliderComponent> GetCurrentFrameBoxes()
	{
		if (currentMotion.frameToBoxes.size() <= 0)
		{
			return {};
		}
		else
		{
			auto it = currentMotion.frameToBoxes.lower_bound(currentActionFrame);

			if (it != currentMotion.frameToBoxes.begin())
			{
				--it;
			}
			if (currentActionFrame >= it->first)
			{
				return it->second;
			}
		}
	}
};

