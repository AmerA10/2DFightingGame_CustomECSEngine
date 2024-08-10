
#include "./LevelLoader.h"
#include <fstream>
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
#include "../Components/ScriptComponent.h"
#include "../Components/AudioComponent.h"
#include "../Components/TestComponent.h"
#include "../Components/FAnimationComponent.h"
#include "../Components/BattleBoxColliderComponent.h"
#include "../Components/FighterComponent.h"
#include "../Components/InputBufferReceiverComponent.h"
#include "./Game.h"
#include "../Animation/AnimationClip.h"
#include "../Events/InputActionEvent.h"
#include <sol/sol.hpp>
#include "../FighterCore/FighterCore.h"

LevelLoader::LevelLoader()
{

}

LevelLoader::~LevelLoader()
{

}


void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level, std::unique_ptr<EventBus>& eventBus)
{
	//This load result, this will NOT execute the script but just loading it
	sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(level) + ".lua");

	//Valid Checks syntax of script but does not execute
	if (!script.valid())
	{
		//The script is therefore a sol::error
		sol::error err = script;
		std::string errorMsg = err.what();

		Logger::Err("error loading lua script with error: " + errorMsg);
		return;

	}

	//Now we execute the script via the Sol lua state
	lua.script_file("./assets/scripts/Level" + std::to_string(level) + ".lua");

	Logger::Log("Opening Lua script");

	sol::table levelTable = lua["Level"];

	////// Read the Level Assets

	sol::table assetsTable = levelTable["assets"];
	

	int i = 0;
	while (true)
	{
		//trying to read an optional table
		sol::optional<sol::table> hasAsset = assetsTable[i];

		if (hasAsset == sol::nullopt)
		{
			break;
		}
		
		sol::table asset = assetsTable[i];

		std::string assetType = asset["type"];
		std::string assetId = asset["id"];
		std::string assetFile = asset["file"];
		//TODO: Need more error checking, make this a switch statement its faster
		if (assetType == "texture")
		{
			assetStore->AddTexture(renderer, assetId, assetFile);
			Logger::Log("added new texture: " + assetId);
		}
		else if (assetType == "font")
		{
			assetStore->AddFont(assetId, assetFile, asset["font_size"].get_or(10));
			Logger::Log("added new Font: " + assetId);
		}
		else if (assetType == "sound")
		{
			assetStore->AddSound(assetId, assetFile);
			Logger::Log("Added new sound: " + assetId);
		}
		else if (assetType == "song")
		{
			assetStore->AddMusic(assetId, assetFile);
			Logger::Log("Added new song: " + assetFile);
		}
		else if (assetType == "animClip")
		{
			float duration = asset["duration"].get_or(1.0);
			std::map<float, int> timeToFrames;
			std::map<float, sol::function> timeToEvents;

			sol::optional<sol::table> hasTimeTable = asset["timeToFrames"];
			if (hasTimeTable != sol::nullopt)
			{
				sol::table timeTable = asset["timeToFrames"];
				int i = 0;
				while (true)
				{
					sol::optional<sol::table> hasFrame = timeTable[i];
					if (hasFrame == sol::nullopt)
					{
						break;
					}
					sol::table frame = timeTable[i];
					float timeStamp = frame["t"].get_or(0.0);

					sol::optional<int> hasValue = frame["f"];

					if (hasValue != sol::nullopt)
					{
						int val = frame["f"].get_or(0);
						timeToFrames.emplace(timeStamp, val);
						Logger::Log("adding to timeline|  t:" + std::to_string(timeStamp) + " f: " + std::to_string(val));
					}

					
					sol::optional<sol::function> func = frame["e"];

					if (func != sol::nullopt)
					{
						sol	::function funcToAdd = frame["e"];
						timeToEvents.emplace(timeStamp, funcToAdd);

					}
					


					i++;
				}

			}
			std::shared_ptr<SpriteAnimationClip> newAnim = std::make_unique<SpriteAnimationClip>(assetId, asset["sprite_sheet_id"].get_or<std::string>(""), timeToFrames, duration, timeToEvents);
			assetStore->AddAnimationClip(assetId, newAnim);

		}
		else if (assetType == "fightAnimationClip")
		{
			int frameDuration = asset["duration"].get_or(1);
			std::map<int, int> frameNumToVal;
			std::map<int, sol::function> frameNumToEvent;

			sol::optional<sol::table> hasTimeTable = asset["frameNumToFrames"];
			if (hasTimeTable != sol::nullopt)
			{
				sol::table frameNumTable = asset["frameNumToFrames"];
				int i = 0;
				while (true)
				{
					sol::optional<sol::table> hasFrame = frameNumTable[i];
					if (hasFrame == sol::nullopt)
					{
						break;
					}
					sol::table frame = frameNumTable[i];

					int timeStamp = frame["t"].get_or(0);

					sol::optional<int> hasValue = frame["f"];

					if (hasValue != sol::nullopt)
					{
						int val = frame["f"].get_or(0);
						frameNumToVal.emplace(timeStamp, val);
						Logger::Log("adding to timeline|  t:" + std::to_string(timeStamp) + " f: " + std::to_string(val));
					}


					sol::optional<sol::function> func = frame["e"];

					if (func != sol::nullopt)
					{
						sol::function funcToAdd = frame["e"];
						frameNumToEvent.emplace(timeStamp, funcToAdd);

					}

					i++;
				}

			}
			FAnimationClip newAnim = { assetId, asset["sprite_sheet_id"].get_or<std::string>(""), frameNumToVal, frameDuration, frameNumToEvent };
			assetStore->AddFAnimationClip(assetId, newAnim);

		}

		else if (assetType == "sprite_sheet")
		{
			assetStore->AddTexture(renderer, assetId, assetFile);

			int numFrames = asset["num_frames"].get_or(0);
			sol::optional<sol::table> hasFrameData = asset["frame_data"];

			std::map<int, SDL_Rect> indexToRect;

			if (hasFrameData != sol::nullopt)
			{
				sol::table frameData = asset["frame_data"];

				int i = 0;
				while (true)
				{
					sol::optional<sol::table> hasFrame = frameData[i];
					if (hasFrame == sol::nullopt)
					{
						break;
					}

					sol::table frame = frameData[i];
					SDL_Rect frameRect = { frame["x"].get_or(0), frame["y"].get_or(0),frame["w"].get_or(0) ,frame["h"].get_or(0)};
					indexToRect.emplace(i, frameRect);

					i++;
				}

				SpriteSheet sheetToAdd = { assetId,numFrames, indexToRect };

				assetStore->AddSpriteSheet(assetId, sheetToAdd);
			}

		}

		else if (assetType == "FightMotion")
		{
			FightMotion motionToAdd;
			motionToAdd.motionId = assetId;
			motionToAdd.motionAction = asset["action"].get_or<std::string>("NONE");
			motionToAdd.motionDuration = asset["duration"].get_or(-1);
			motionToAdd.motionType = (MotionType)asset["motion_type"].get_or(0);
			motionToAdd.canCancel = asset["can_cancel"];
			motionToAdd.motionAnimClipId = asset["anim_clip_id"].get_or <std::string>("");

			std::map<int, std::vector<BoxColliderComponent>> framesToColliderMapToAdd;
			std::vector<BoxColliderComponent> boxesToAdd;
			sol::optional<sol::table> hasBoxesTable = asset["framesToBoxes"];

			//this is a nightmare and a half but it works so dont touch it basically
			if (hasBoxesTable != sol::nullopt)
			{
				sol::table framesToBoxesTable = asset["framesToBoxes"];

				int i = 0;
				while (true)
				{
					sol::optional<sol::table> hasBoxesTable = framesToBoxesTable[i];
					if (hasBoxesTable == sol::nullopt)
					{
						break;
					}
					sol::table framesToBoxTable = framesToBoxesTable[i];
					int time = framesToBoxTable["t"];
					sol::optional<sol::table> hasBoxesArray = framesToBoxTable["boxes"];
					if (hasBoxesArray != sol::nullopt)
					{
						sol::table boxesArray = framesToBoxTable["boxes"];
						int j = 0;
						while (true)
						{
							sol::optional<sol::table> hasBox = boxesArray[j];
							if (hasBox == sol::nullopt)
							{
								break;
							}
							sol::table collider = boxesArray[j];
							BoxColliderComponent boxToAdd = BoxColliderComponent
							{
								collider["width"].get_or(1),collider["height"].get_or(1),
								glm::vec2(collider["scale"]["x"],collider["scale"]["y"]),
								glm::vec2(collider["offset"]["x"],collider["offset"]["y"]),
								collider["layer"].get_or<std::string>(""),
								collider["mask"].get_or<std::string>(""),
							};
							boxesToAdd.push_back(boxToAdd);
							j++;
						}
					}
					framesToColliderMapToAdd.emplace(time, boxesToAdd);
					i++;

				}
				motionToAdd.frameToBoxes = framesToColliderMapToAdd;
				assetStore->AddMotion(assetId, motionToAdd);
			}
		
		}
		else if (assetType == "Fighter")
		{
			Fighter fighter;
			std::vector<std::string> motionsVec;
			fighter.id = assetId;
			fighter.backMoveSpeed = asset["back_move_speed"];
			fighter.forwardMoveSpeed= asset["forward_move_speed"];

			sol::optional<sol::table> hasMotions = asset["motions"];

			if (hasMotions != sol::nullopt)
			{
				sol::table motions = asset["motions"];
				int i = 0;
				while (true)
				{
					sol::optional<sol::table> hasMotion = motions[i];

					if (hasMotion == sol::nullopt)
					{
						break;
					}
					sol::table motion = motions[i];
					std::string motionId = motion["id"].get_or<std::string>("");
					motionsVec.push_back(motionId);
					i++;
				}
			}

			fighter.motions = motionsVec;

			assetStore->AddFighter(fighter.id, fighter);



		}

		i++;
	}

	sol::table mapTable = levelTable["tilemap"];

	std::string mapFilePath = mapTable["map_file"];
	std::string mapTextureId = mapTable["texture_asset_id"];
	int mapNumRows = mapTable["num_rows"];
	int mapNumCols = mapTable["num_cols"];
	int mapTileSize = mapTable["tile_size"];
	double mapTileScale = mapTable["scale"];


	//TODO: We want to load everything from the Lua script file

	std::fstream mapFile;
	mapFile.open(mapFilePath);

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * mapTileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * mapTileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (mapTileScale * mapTileSize), y * (mapTileScale * mapTileSize)), glm::vec2(mapTileScale, mapTileScale), 0.0);
			tile.AddComponent<SpriteComponent>(mapTextureId, mapTileSize, mapTileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();

	Game::mapWidth = mapNumCols * mapTileSize * (int)mapTileScale;
	Game::mapHeight = mapNumRows * mapTileSize * (int)mapTileScale;

	SDL_Color startColor = { 10,255,10 };

	sol::table entTable = levelTable["entities"];
	i = 0;

	while (true)
	{
		sol::optional<sol::table> hasEntity = entTable[i];
		if (hasEntity == sol::nullopt)
		{
			break;
		}

		sol::table entity = entTable[i];

		Entity newEntity = registry->CreateEntity();

		sol::optional<std::string> tag = entity["tag"];
		if (tag != sol::nullopt)
		{
			newEntity.Tag(entity["tag"]);
		}

		sol::optional<std::string> group = entity["group"];
		if (group != sol::nullopt)
		{
			newEntity.Group(entity["group"]);
		}

		sol::optional<sol::table> compTable = entity["components"];
		if (compTable != sol::nullopt)
		{
			//Now need to check for literally every single component possible
			sol::optional<sol::table> transComp = entity["components"]["transform"];
			if (transComp != sol::nullopt)
			{
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(
						entity["components"]["transform"]["position"]["x"].get_or(0.0),
						entity["components"]["transform"]["position"]["y"].get_or(0.0)
					),
					glm::vec2(
						entity["components"]["transform"]["scale"]["x"].get_or(1.0),
						entity["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					entity["components"]["transform"]["rotation"].get_or(0.0)

					);
			}

			sol::optional<sol::table> rbComp = entity["components"]["rigidbody"];
			if (rbComp != sol::nullopt)
			{
				newEntity.AddComponent<RigidBodyComponent>(
					glm::vec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
					)
					);

			}

			sol::optional<sol::table> spriteComp = entity["components"]["sprite"];
			if (spriteComp != sol::nullopt)
			{
				newEntity.AddComponent<SpriteComponent>(

					entity["components"]["sprite"]["texture_asset_id"].get_or<std::string>(""),
					entity["components"]["sprite"]["width"].get_or(20),
					entity["components"]["sprite"]["height"].get_or(20),
					entity["components"]["sprite"]["z_index"].get_or(0),
					entity["components"]["sprite"]["fixed"].get_or(false),
					entity["components"]["sprite"]["src_rect_x"].get_or(0),
					entity["components"]["sprite"]["src_rect_y"].get_or(0)

					);
			}

			sol::optional<sol::table> animComp = entity["components"]["animation"];
			if (animComp != sol::nullopt)
			{
				newEntity.AddComponent<AnimationComponent>(

					entity["components"]["animation"]["animation_clip_id"].get_or<std::string>(""),
					entity["components"]["animation"]["playback_rate"].get_or(1.0),
					entity["components"]["animation"]["num_loops"].get_or(0)

					);
			}

			sol::optional<sol::table> fAnimComp = entity["components"]["f_animation"];
			if (fAnimComp != sol::nullopt)
			{
				if (!newEntity.HasComponent<AnimationComponent>())
				{
					newEntity.AddComponent<FAnimationComponent>(
						entity["components"]["f_animation"]["clip_id"].get_or<std::string>(""),
						entity["components"]["f_animation"]["playback_rate"].get_or(1)
						);
				}
			}

			sol::optional<sol::table> boxCollComp = entity["components"]["boxcollider"];
			std::string layer = entity["components"]["boxcollider"]["layer"].get_or<std::string>("00000000000000000000000000000000");
			std::string mask = entity["components"]["boxcollider"]["mask"].get_or<std::string>("00000000000000000000000000000000");
			if (boxCollComp != sol::nullopt)
			{
				newEntity.AddComponent<BoxColliderComponent>(

					entity["components"]["boxcollider"]["width"].get_or(1),
					entity["components"]["boxcollider"]["height"].get_or(1),
					glm::vec2(
						entity["components"]["boxcollider"]["scale"]["x"].get_or(1),
						entity["components"]["boxcollider"]["scale"]["y"].get_or(1)

					),
					glm::vec2(
						entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
						entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
					),
					layer,
					mask
					);
			}

			sol::optional<sol::table> hasBattleBoxComp = entity["components"]["battle_box_colliders"];
			if (hasBattleBoxComp != sol::nullopt)
			{
				std::vector<BoxColliderComponent> boxes;
				sol::optional<sol::table> hasColliders = entity["components"]["battle_box_colliders"]["colliders"];

				sol::table colliders;
				if (hasColliders != sol::nullopt)
				{
					colliders = entity["components"]["battle_box_colliders"]["colliders"];
					int i = 0;
					while (true)
					{
						sol::optional<sol::table> hasCollider = colliders[i];
						if (hasCollider == sol::nullopt)
						{
							break;
						}
						sol::table collider = colliders[i];
						BoxColliderComponent boxToAdd =
						{
							collider["width"].get_or(1),
							collider["height"].get_or(1),
							glm::vec2(
								collider["scale"]["x"].get_or(1),
								collider["scale"]["y"].get_or(1)

							),
							glm::vec2(
							collider["offset"]["x"].get_or(0),
							collider["offset"]["y"].get_or(0)
							),
							collider["layer"].get_or<std::string>("00000000000000000000000000000000"),
							collider["mask"].get_or<std::string>("00000000000000000000000000000000")
						};

						boxes.emplace_back(boxToAdd);
						i++;
					}
					newEntity.AddComponent<BattleBoxColliderComponent>(boxes);

				}
				
			}

			sol::optional<sol::table> healthComp = entity["components"]["health"];
			if (healthComp != sol::nullopt)
			{
				newEntity.AddComponent<HealthComponent>(

					static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))

					);
			}

			sol::optional<sol::table> projEmitComp = entity["components"]["projectile_emitter"];
			if (projEmitComp != sol::nullopt)
			{
				newEntity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(
						entity["components"]["projectile_emitter"]["projectile_velocity"]["x"].get_or(0),
						entity["components"]["projectile_emitter"]["projectile_velocity"]["y"].get_or(0)

					),
					(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(10)) * 1000,
					(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(100)) * 1000,
					entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10),
					entity["components"]["projectile_emitter"]["friendly"].get_or(true),
					entity["components"]["projectile_emitter"]["projectile_speed"].get_or(10),
					entity["components"]["projectile_emitter"]["use_frequency"].get_or(true)

					);
			}

			sol::optional<sol::table> keyboardConComp = entity["components"]["keyboard_controller"];
			if (keyboardConComp != sol::nullopt)
			{
				newEntity.AddComponent<KeyboardControlledComponent>(
					glm::vec2(
						entity["components"]["keyboard_controller"]["up_velocity"]["x"].get_or(0),
						entity["components"]["keyboard_controller"]["up_velocity"]["y"].get_or(0)

					),
					glm::vec2(
						entity["components"]["keyboard_controller"]["right_velocity"]["x"].get_or(0),
						entity["components"]["keyboard_controller"]["right_velocity"]["y"].get_or(0)

					)

					);
			}

			sol::optional<sol::table> cameraFollowComp = entity["components"]["camera_follow"];
			if (cameraFollowComp != sol::nullopt)
			{
				newEntity.AddComponent<CameraFollowComponent>(

					entity["components"]["camera_follow"]["follow"].get_or(true)

					);
			}

			sol::optional<sol::table> textLabelComp = entity["components"]["text_label"];
			if (textLabelComp != sol::nullopt)
			{
				SDL_Color color = {
					entity["components"]["text_label"]["color"]["r"].get_or(0),
					entity["components"]["text_label"]["color"]["g"].get_or(0),
					entity["components"]["text_label"]["color"]["b"].get_or(0)
				};
				glm::vec2 newEntityPos = newEntity.GetComponent<TransformComponent>().position;
				newEntity.AddComponent<TextLabelComponent>(

					entity["components"]["text_label"]["font_asset_id"].get_or<std::string>(""),
					entity["components"]["text_label"]["text"].get_or<std::string>(""),
					glm::vec2(
						entity["components"]["text_label"]["text_position"]["x"].get_or(newEntityPos.x),
						entity["components"]["text_label"]["text_position"]["y"].get_or(newEntityPos.y)

					),
					color,
					entity["components"]["text_label"]["is_fixed"].get_or(true)

					);
			}

			sol::optional<sol::table> soundComp = entity["components"]["sound"];
			if (soundComp != sol::nullopt)
			{
				newEntity.AddComponent<AudioComponent>(
					entity["components"]["sound"]["sound_asset_id"].get_or<std::string>(""),
					entity["components"]["sound"]["sound_loop"].get_or(false),
					entity["components"]["sound"]["is_music"].get_or(false),
					entity["components"]["sound"]["sound_volume"].get_or(25)
					
					);
			}

			sol::optional<sol::table> updateScriptComp = entity["components"]["on_update_script"];
			if (updateScriptComp != sol::nullopt)
			{
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}

			sol::optional<sol::table> fighterComp = entity["components"]["fighter"];
			if (fighterComp != sol::nullopt)
			{
				newEntity.AddComponent<FighterComponent>(
					entity["components"]["fighter"]["fighter_id"].get_or<std::string>("")
					);
			}
			sol::optional<sol::table> inputComp = entity["components"]["input_receiver"];
			if (inputComp != sol::nullopt)
			{
				newEntity.AddComponent<InputBufferReceiverComponent>(
					entity["components"]["input_receiver"]["input_map_id"].get_or<std::string>("")
					);
			}

		}
		
		i++;

	}

}