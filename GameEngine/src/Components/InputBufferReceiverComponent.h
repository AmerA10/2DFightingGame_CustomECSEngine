#pragma once
#include <string>
#include "../Input/InputMap.h"
#include <SDL.h>
const int MAX_BUFFER_ELEMENTS = 20;
const int MIN_BUFFER_DISTANCE = 10;
struct InputBufferReceiverComponent
{
	std::string inputMapId;

	InputMap inputMap;

	std::vector<SDL_Keycode> keys;

	int head;
	int tail;


	std::vector<InputAction> actionBuffer;

public:
	InputBufferReceiverComponent(const std::string& inputMapId = "", InputMap inputMap = {}, std::vector<SDL_Keycode> keys = {})
	{
		this->inputMapId = inputMapId;
		this->inputMap = inputMap;
		this->keys = keys;
		this->head = 0; 
		this->tail = 0;
		this->actionBuffer.reserve(MAX_BUFFER_ELEMENTS);
		this->actionBuffer.resize(MAX_BUFFER_ELEMENTS);
	}

	void UpdateKeys(std::vector<SDL_Keycode>& keysToAdd)
	{
		this->keys.clear();
		this->keys.insert(this->keys.end(), keysToAdd.begin(), keysToAdd.end());

		//go through every key and see if any correspond to an action
		if (keys.size() <= 0)
		{
			InputAction idleAction = { "NONE" };
			AddToBuffer(idleAction);
			return;
		}

		for (const SDL_Keycode key : keys)
		{
			if (inputMap.inputMap.find(key) != inputMap.inputMap.end())
			{
				AddToBuffer(inputMap.inputMap.at(key));
			}
		}

	}

	//InputAction GetCurrentAction() const
	//{

	//	InputAction result = { "NONE" };

	//	if (keys.size() <= 0)
	//	{
	//		return result;
	//	}

	//	const SDL_Keycode& lastKey = keys[keys.size() - 1];
	//	if (inputMap.inputMap.find(lastKey) != inputMap.inputMap.end())
	//	{
	//		result = inputMap.inputMap.at(lastKey);
	//		
	//	}
	//	
	//	return result;
	//}

	InputAction GetCurrentAction()
	{
		//for now lets just return the tail
		InputAction result = actionBuffer[this->head];
		int reader = head;
		while (reader != tail)
		{
			result = actionBuffer[reader];
			reader = (reader + 1) % MAX_BUFFER_ELEMENTS;

		}
		return result;

		
	}

	void SetupInputMap(const std::unique_ptr<InputMap>& inputMapToAdd)
	{
		//this is not the way this is supposed to work lmao this should be a shared pointer or something
		this->inputMap.inputMapId = inputMapToAdd->inputMapId;
		this->inputMap.inputMap = inputMapToAdd->inputMap;
	}

	void AddToBuffer(const InputAction action)
	{
		actionBuffer[tail] = action;
		tail = (tail + 1) % MAX_BUFFER_ELEMENTS;

		int currentDistance = 0;
		if (head > tail)
		{
			currentDistance = ((MAX_BUFFER_ELEMENTS + 1) - head) + tail;

			if ((currentDistance - MIN_BUFFER_DISTANCE) > 0)
			{
				head = (head + (currentDistance - MIN_BUFFER_DISTANCE)) % MAX_BUFFER_ELEMENTS;
			}
		}
		else if (tail > head)
		{
			currentDistance = (tail - head) + 1;
			if ((currentDistance - MIN_BUFFER_DISTANCE) > 0)
			{
				head = head + (currentDistance - MIN_BUFFER_DISTANCE) % MAX_BUFFER_ELEMENTS;
			}
		}
		 
		if (action.inputActionName != "NONE")
		{
			Logger::Log("Adding to buffer: " + action.inputActionName + " head: " + std::to_string(head) + " tail: " + std::to_string(tail));
		}
	}

};