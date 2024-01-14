#include "Game.h"
#include <iostream>


Game::Game() 
{
	window = NULL;
	renderer = NULL;
	std::cout << "Game Constructor called!" << std::endl;
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

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		1920, 
		1080,
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

}

void Game::ProcessInput() 
{

}

void Game::Run() 
{
	while (true) 
	{
		
	}
}

void Game::Update() 
{

}

void Game::Destroy() 
{

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}



