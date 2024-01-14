#include "Game.h"
#include <iostream>
#include <SDL.h>


Game::Game() 
{
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

}



