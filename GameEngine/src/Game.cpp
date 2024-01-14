#include "Game.h"
#include <iostream>


Game::Game() 
{
	window = NULL;
	renderer = NULL;
	std::cout << "Game Constructor called!" << std::endl;
	isRunning = false;
}


Game::~Game() 
{
}

void Game::Initialize() 
{

	//Init every system that sdl can do
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error Init SDL" << std::endl;
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
		std::cerr << "Error Creating SDL Window" << std::endl;
		return;
	}

	//We pass -1 because it gets the default window index
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) 
	{
		std::cerr << "Error Creating SDL Renderer" << std::endl;
		return;
	}

	SDL_SetWindowFullscreen(window, 0);

	isRunning = true;

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
	while (isRunning) 
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Update() 
{

}

void Game::Render() 
{
	SDL_SetRenderDrawColor(renderer, 50, 125, 150, 255);
	
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Destroy() 
{

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}