#include "InputLoader.h"
#include "../Systems/InputBufferSystem.h"
#include "../AssetStore/AssetStore.h"


InputLoader::InputLoader()
{

}
InputLoader::~InputLoader()
{

}

void InputLoader::LoadInput(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore)
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

	sol::table mappingsTable = lua["InputMapping"]["general_mappings"];

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

	sol::optional<sol::table> hasIndividualMappingsTable = lua["InputMapping"]["individual_input_mappings"];
	if (hasIndividualMappingsTable != sol::nullopt)
	{
		sol::table individualMappingsTable = lua["InputMapping"]["individual_input_mappings"];

		int j = 0;
		while (true)
		{
			sol::optional<sol::table> hasMappingTable = individualMappingsTable[j];
			if (hasMappingTable == sol::nullopt)
			{
				break;
			}

			sol::table mappingsTable = individualMappingsTable[j];

			std::string mappingTableId = mappingsTable["id"];

			sol::optional<sol::table> hasKeyActionMappingTable = mappingsTable["mapping"];
			sol::table keyActionMappingTable = mappingsTable["mapping"];
			InputMap mapToAdd;
			mapToAdd.inputMapId = mappingTableId;
			if (hasKeyActionMappingTable != sol::nullopt)
			{
				int y = 0;
				while (true)
				{

					sol::optional<sol::table> hasInputMapData = keyActionMappingTable[y];
					if (hasInputMapData == sol::nullopt)
					{
						break;
					}
					sol::table inputMapData = keyActionMappingTable[y];
					std::string keyStr = inputMapData["key"];
					SDL_Keycode keyCode = SDL_GetKeyFromName(keyStr.c_str());
					InputAction actionToAdd = { inputMapData["action"], inputMapData["scale"].get_or(1) };
					mapToAdd.inputMap.emplace(keyCode,actionToAdd);
					y++;
				}
			}

			assetStore->AddInputMap(mapToAdd.inputMapId, mapToAdd);
			j++;

;		}
	}

	Logger::Log("Number of mappings created: " + std::to_string(registry->GetSystem<InputBufferSystem>().inputActionMapping.size()));

}