#include "Game.h"
#include <iostream>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include "../logger/Logger.h"


Game::Game() 
{
	window = NULL;
	renderer = NULL;
	isRunning = false;
	millisecondsPreviousFrame = 0;
	deltaTime = 0.0;
	windowWidth = 0;
	windowHeight = 0;

	Logger::Log("game constructor is called");

}


Game::~Game() 
{
}

void Game::Initialize() 
{

	//Init every system that sdl can do
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error Init SDL");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight= displayMode.h;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) 
	{
		Logger::Err("Error Creating SDL Window");
		return;
	}

	//We pass -1 because it gets the default window index
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) 
	{
		Logger::Err("Error Creating SDL Renderer");
		return;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
	Logger::Log("game init success");

}

void Game::Setup()
{
	//TOOD:
	//Entity Tank = registry.CreateEntity();
	//tank.AddComponent<TrasnformComponent>();
	//tank.AddBoxCollider>();
	//tank.AddComponent<SpriteComponent>("./assets/images/tank.png");

}

void Game::ProcessInput() 
{
	SDL_Event sdlEvent;
	
	//(Maybe) While this will provide every single event available, there are methods of filtering out
	//The events so that only those events that are input related can be processed
	while (SDL_PollEvent(&sdlEvent)) 
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
			{
				isRunning = false;
			}
			break;
		}
	}
}

void Game::Run() 
{

	Setup();
	while (isRunning) 
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Update() 
{
	//TODO: if we are too fast, we need to waste some time until we reach the target time
	//Each frame should take
	//Another thing is that this isnt the best solution, it is quite simple and there are better approaches 
	//That exist that are more robust
	
	//calculate time elapsed since last frame in terms of seconds

	//This is how many milliseconds since initialization of SDL


	//check if the amount of time passed since the last frame
	//is greater than the current time + the time each frame should take
	//This approach does waste clock cycles
	/*while (!SDL_TICKS_PASSED(SDL_GetTicks64(), millisecondsPreviousFrame + MILLISECS_PER_FRAME)) 
	{

	}*/
	Uint64 timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecondsPreviousFrame);


	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		
		SDL_Delay(timeToWait);

	}
	deltaTime = (SDL_GetTicks64() - millisecondsPreviousFrame) / (double)1000.0;

	millisecondsPreviousFrame = SDL_GetTicks64();

	//TOOD:
	// Actually probably we want something else to call update on all systems since the idea of
	// systems can grow quite big
	//MovementSystem.Update();

}

void Game::Render() 
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	//Draw png texture, SDL does not know how to read png filess only bitmaps
	//It is why we have the SDL_Image included


	SDL_RenderPresent(renderer);
}

void Game::Destroy() 
{

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	Logger::Log("game destructor called");

}