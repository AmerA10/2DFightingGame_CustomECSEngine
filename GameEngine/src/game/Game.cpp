#include "Game.h"
#include <iostream>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include "../logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderDebugSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardInputSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitterSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include <fstream>
#include <sstream>
#include <array>
#include <streambuf>

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

void Game::LoadTileMap(const std::string& mapPath, const std::string& texturePath) {

	assetStore->AddTexture(renderer, "tile-Image", texturePath);

	int tileWidth = 32;
	int tileHeight = 32;

	
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	// Load the tilemap
	int tileSize = 32;
	double tileScale = 5.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0,false, srcRectX, srcRectY);
		}
	}
	mapFile.close();

	mapWidth = mapNumCols * tileSize * tileScale;;
	mapHeight = mapNumRows * tileSize * tileScale;

}

void Game::LoadLevel(int level) 
{
	LoadTileMap("./assets/tilemaps/jungle.map", "./assets/tilemaps/jungle.png");

	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-down.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
	assetStore->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 30);
	assetStore->AddFont("arial-font", "./assets/fonts/arial.ttf", 30);

	//Add systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderDebugSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardInputSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectilEmitterSystem>();
	registry->AddSystem<ProjectileLifeCycleSystem>();
	registry->AddSystem<RenderTextSystem>();


	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(200.0, 200.0), glm::vec2(2.0, 2.0), 0.0f);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32,2);
	tank.AddComponent<BoxColliderComponent>(32,32, tank.GetComponent<TransformComponent>().scale);
	tank.Group("Enemies");
	Entity truck = registry->CreateEntity();
	tank.AddComponent<HealthComponent>(50);


	//This shows that even constructor with default variables
	truck.AddComponent<TransformComponent>(glm::vec2(500.0,500.0));
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0,0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 64, 64, 1);
	truck.AddComponent<BoxColliderComponent>(64,64,truck.GetComponent<TransformComponent>().scale);
	truck.AddComponent<HealthComponent>(100);
	truck.Group("Enemies");
	truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0, -100), 1000, 7000, 10, false, 80, true);
	truck.AddComponent<HealthComponent>(50);

	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 500.0), glm::vec2(2.0, 2.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, -50.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2,20,true);
	chopper.AddComponent<BoxColliderComponent>(32, 32, chopper.GetComponent<TransformComponent>().scale);
	chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0,-400), glm::vec2(400, 0), glm::vec2(0, 400), glm::vec2(-400, 0));
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(100,0),1000,5000,5,true, 200,false);
	chopper.AddComponent<CameraFollowComponent>();
	chopper.Tag("Player");
	chopper.Group("Military");
	chopper.AddComponent<HealthComponent>(20);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 200, 50), glm::vec2(2.0, 2.0), 0.0);
	radar.AddComponent<RigidBodyComponent>();
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 3, true);
	radar.AddComponent<AnimationComponent>(8, 3, true);
	radar.Group("UI");
	

	//This won't use the same ID as above simply because its all happening in one frame, you need the death
	//and creation to be on different frames, Frame in between the death and creation

	//Entity newRader = registry->CreateEntity();
	//newRader.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(2.0, 2.0), 0.0);

	Entity label1 = registry->CreateEntity();
	SDL_Color white = { 255,255,255 };
	label1.AddComponent<TextLabelComponent>("charriot-font", "Sample Text 1", glm::vec2(500,50), white);

	Entity label2 = registry->CreateEntity();
	SDL_Color purple = { 255,0,255 };
	label2.AddComponent<TextLabelComponent>("arial-font", "Sample Text 2", glm::vec2(500, 100), purple);

}

void Game::Setup()
{

	LoadLevel(1);
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
	renderer = SDL_CreateRenderer(window, -1, 0);

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

	isRunning = true;
	Logger::Log("game init success");

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

			eventBus->EmitEvent<KeyboardInputEvent>(sdlEvent.key.keysym.sym);

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
	deltaTime = (float)(SDL_GetTicks64() - millisecondsPreviousFrame) / 1000.0f;

	millisecondsPreviousFrame = (int)SDL_GetTicks64();

	//I Do not like the idea of reseting the subscribers and constantly 
	//Subscribing and reseting the event bus subscribers
	//But currently we have to do this because there is no way
	//Of the subscibing events or systems that have subscribed to events
	//in the event bus class

	eventBus->Reset();
	
	registry->GetSystem<DamageSystem>().SubscriberToEvents(eventBus);
	registry->GetSystem<KeyboardInputSystem>().SubscribeToKeyInputEvent(eventBus);
	registry->GetSystem<ProjectilEmitterSystem>().SubscribeToKeyInputEvent(eventBus);
	//Update the registry to process the entities that are waiting to be created/deleted
	registry->Update();

	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectilEmitterSystem>().Update(registry);
	registry->GetSystem<ProjectileLifeCycleSystem>().Update();
}

void Game::Render() 
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	//Draw png texture, SDL does not know how to read png filess only bitmaps
	//It is why we have the SDL_Image included
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore,camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);

	if (drawDebug) {
		registry->GetSystem<RenderDebugSystem>().Update(renderer,camera);
	}

	SDL_RenderPresent(renderer);
}

void Game::Destroy() 
{

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