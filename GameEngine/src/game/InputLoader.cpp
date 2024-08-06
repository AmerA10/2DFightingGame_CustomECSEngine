#include "InputLoader.h"
#include "../Systems/InputBufferSystem.h"


InputLoader::InputLoader()
{

}
InputLoader::~InputLoader()
{

}

void InputLoader::LoadInput(sol::state& lua, const std::unique_ptr<Registry>& registry)
{
	sol::load_result script = lua.load_file("./assets/scripts/InputMapping.lua");

	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMsg = err.what();

		Logger::Err("error loading lua script with error: " + errorMsg);
		return;

	}

	lua.script_file("./assets/scripts/InputMapping.lua");

	sol::table mappingsTable = lua["InputMapping"]["mappings"];

	int i = 0;
	while (true)
	{
		sol::optional<sol::table> hasMapping = mappingsTable[i];

		if (hasMapping == sol::nullopt)
		{
			break;
		}

		sol::table mapping = mappingsTable[i];
		std::string key = mapping["key"];
		std::string action = mapping["action"];
		int scale = mapping["scale"].get_or(1);


		SDL_Keycode code = SDL_GetKeyFromName(key.c_str());
		std::string keyName = SDL_GetKeyName(code);

		i++;



		if (code == SDLK_UNKNOWN)
		{
			Logger::Err("Cant do it man");
			continue;
		}

		registry->GetSystem<InputBufferSystem>().AddInputActionMapping(code,action,scale);

		Logger::Log("Creating input mapping for key: " + key + " Action: " + action);

	}
	Logger::Log("Number of mappings created: " + std::to_string(registry->GetSystem<InputBufferSystem>().inputActionMapping.size()));

}