#include "Game.h"
#include "LevelLoader.h"
#include "InputLoader.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include "../logger/Logger.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/BattleCollisionSystem.h"
#include "../Systems/RenderDebugSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardInputSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitterSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/HealthDisplaySystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/ScriptSystem.h"
#include "../Systems/AudioSystem.h"
#include "../Systems/InputBufferSystem.h"
#include "../Systems/TestSystem.h"
#include "../Systems/FAnimationSystem.h"
#include "../Systems/RenderDebugBattleSystem.h"
#include "../Input/Input.h"
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <vector>


int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() 
{
	window = NULL;
	renderer = NULL;
	isRunning = false;
	drawDebug = true;
	millisecondsPreviousFrame = 0;
	deltaTime = 0.0;
	windowWidth = 0;
	windowHeight = 0;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
	Logger::Log("game constructor is called");

}


void Game::Setup()
{
	//Add systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<BattleCollisionSystem>();
	registry->AddSystem<RenderDebugSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardInputSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectilEmitterSystem>();
	registry->AddSystem<ProjectileLifeCycleSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<HealthDisplaySystem>();
	registry->AddSystem<RenderGUISystem>();
	registry->AddSystem<RenderDebugBattleSystem>();
	registry->AddSystem<ScriptSystem>();
	registry->AddSystem<AudioSystem>();
	registry->AddSystem<InputBufferSystem>();
	registry->AddSystem<TestSystem>();
	registry->AddSystem<FAnimationSystem>();

	eventBus->Reset();

	
	registry->GetSystem<ScriptSystem>().CreateLuaBinding(lua);

	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

	LevelLoader loader;

	loader.LoadLevel(lua, registry, assetStore,renderer,1, eventBus);

	InputLoader inputLoader;
	inputLoader.LoadInput(lua, registry);


	//this works though we can have an event just subscribed to once and done
	registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<DamageSystem>().SubscriberToEvents(eventBus);
	registry->GetSystem<KeyboardInputSystem>().SubscribeToKeyInputEvent(eventBus);
	registry->GetSystem<ProjectilEmitterSystem>().SubscribeToKeyInputEvent(eventBus);


}


void Game::Initialize() 
{

	//Init every system that sdl can do
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error Init SDL");
		return;
	}

	//Need to intialize SDL_TTF
	if (TTF_Init() != 0)
	{
		Logger::Err("Error Intitializing SDL TTF");
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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	if (!renderer) 
	{
		Logger::Err("Error Creating SDL Renderer");
		return;
	}
	

	//Our camera needs a main entity to follow
	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;



	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);



	//Initialize the imGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);


	io = ImGui::GetIO();
	Logger::Log("game init success");
	isRunning = true;

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

void Game::ProcessInput() 
{
	SDL_Event sdlEvent;
	
	//(Maybe) While this will provide every single event available, there are methods of filtering out
	//The events so that only those events that are input related can be processed
	while (SDL_PollEvent(&sdlEvent)) 
	{
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		io.MousePos = ImVec2(mouseX, mouseY);
		//bit wise operations with the button returned
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:

			registry->GetSystem<InputBufferSystem>().Update(eventBus, sdlEvent.key.keysym.sym);

			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
			{
				isRunning = false;
			}
			if (sdlEvent.key.keysym.sym == SDLK_k) 
			{
				drawDebug = !drawDebug;
			}

			break;

		
		
			
		}
		
			
		
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
	deltaTime = (float)(SDL_GetTicks64() - millisecondsPreviousFrame) / 1000.0f;

	millisecondsPreviousFrame = (int)SDL_GetTicks64();

	//I Do not like the idea of reseting the subscribers and constantly 
	//Subscribing and reseting the event bus subscribers
	//But currently we have to do this because there is no way
	//Of the subscibing events or systems that have subscribed to events
	//in the event bus class

	//Update the registry to process the entities that are waiting to be created/deleted
	registry->Update();

	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<BattleCollisionSystem>().Update(eventBus);
	registry->GetSystem<MovementSystem>().Update(registry, deltaTime);
	registry->GetSystem<AnimationSystem>().Update(assetStore);
	registry->GetSystem<FAnimationSystem>().Update(assetStore);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectilEmitterSystem>().Update(registry);
	registry->GetSystem<ProjectileLifeCycleSystem>().Update();
	registry->GetSystem<AudioSystem>().Update(assetStore);
	registry->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
	//registry->GetSystem<TestSystem>().Update();

}

void Game::Render() 
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	//Draw png texture, SDL does not know how to read png filess only bitmaps
	//It is why we have the SDL_Image included
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore,camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
	registry->GetSystem<HealthDisplaySystem>().Update(renderer, camera);
	if (drawDebug) {
		registry->GetSystem<RenderDebugSystem>().Update(renderer,camera);

		//We want to creat e asystem responsible for creating
		//ImGUI widgets
		registry->GetSystem<RenderGUISystem>().Update(registry);
		registry->GetSystem<RenderDebugBattleSystem>().Update(renderer, camera);

	}


	SDL_RenderPresent(renderer);
}

void Game::Destroy() 
{

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//avoide memory leak
	//delete registry;
	Logger::Log("game destructor called");

}
Game::~Game()
{
}