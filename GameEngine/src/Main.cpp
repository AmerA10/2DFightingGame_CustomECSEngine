// GameEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include "./game/Game.h"

//sol already includes lua
//#include <lua/lua.h>
#include <sol/sol.hpp>

void TestLua() 
{
    sol::state lua;
    lua.open_libraries();
    lua.script_file("./assets/scripts/myscript.lua");

    sol::table configTable = lua["config"];

    int width = configTable["resolution"]["width"];
    int height = configTable["resolution"]["height"];

    std::cout << "Resolution" << std::to_string(width)<< std::endl;

    
}

int main(int argc, char* argv[])
{
    //TODO: Do some magic!
    //We want to divide thee responsibilities of our files so we should have a game class

    //Game game;

    //game.Initialize();
    //game.Run();
    //game.Destroy();

    TestLua();

    return 0;
}


