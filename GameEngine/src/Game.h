#pragma once
#include <SDL.h>

//This is protectin as we want to include this file in other places
//This will make the preprocessor include the header file only once
#ifndef GAME_H
#define GAME_H



class Game
{
	private:
		//We can forward declare these things as it is better for performance but thats for later
		SDL_Window* window;
		SDL_Renderer* renderer;


	public:
		Game();
		~Game();
		
		void Initialize();
		void ProcessInput();
		void Update();
		void Run();
		void Destroy();

};


#endif // !GAME_H