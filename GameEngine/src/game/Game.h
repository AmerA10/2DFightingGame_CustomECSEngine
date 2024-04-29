#pragma once
//This is protectin as we want to include this file in other places
//This will make the preprocessor include the header file only once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ECS/ECS.h"
#include <memory>
#include "../AssetStore/AssetStore.h"


const int FPS = 60;
//The time each frame is expected to take
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
	private:
		//We can forward declare these things as it is better for performance but thats for later
		bool isRunning;

		//Why did we replace the Registry pointer with a smart pointer
		//but not these two? well SDL is a c library so it expects
		//raw c pointers, SDL and pointers are opaque in a way
		//We do not know how to creat them and instead
		//we use functions to create them, ex: window is created with sdl_creaateWindow function
		//Mixing the smart and raw pointers will be a bad idea
		SDL_Window* window;
		SDL_Renderer* renderer;

		int millisecondsPreviousFrame = 0;

		float deltaTime = 0;

		std::unique_ptr<Registry> registry;
		std::unique_ptr<AssetStore> assetStore;

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
		void LoadLevel(int level);
		void LoadTileMap(const std::string& mapPath, const std::string& texturePath);

		int windowWidth = 0;
		int windowHeight = 0;

};


#endif // !GAME_H