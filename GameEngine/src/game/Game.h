#pragma once
//This is protectin as we want to include this file in other places
//This will make the preprocessor include the header file only once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ECS/ECS.h"


const int FPS = 60;
//The time each frame is expected to take
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
	private:
		//We can forward declare these things as it is better for performance but thats for later
		bool isRunning;
		SDL_Window* window;
		SDL_Renderer* renderer;

		int millisecondsPreviousFrame = 0;

		float deltaTime = 0;

		Registry* registry;

	public:
		Game();
		~Game();
		void Initialize();
		void ProcessInput();
		void Update();
		void Run();
		void Destroy();
		void Render();
		void Setup();

		int windowWidth = 0;
		int windowHeight = 0;

};


#endif // !GAME_H