// GameEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include "./game/Game.h"
#include "Animation/AnimationClip.h"


int main(int argc, char* argv[])
{
    //TODO: Do some magic!
    //We want to divide thee responsibilities of our files so we should have a game class

    Game game;


    game.Initialize();
    game.Run();
    game.Destroy();


    return 0;
}


