#include "AssetStore.h"
#include "../logger/Logger.h"
#include <SDL_image.h>

AssetStore::AssetStore() 
{
	Logger::Log("Asset Store Constructor");
}

AssetStore::~AssetStore() 
{
	Logger::Log("Asset Store Destructor");
	
	ClearAssets();
	
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) 
{
	Logger::Log("Adding Texture with name " + assetId + " path: " + filePath);

	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	//Add texture to the map
	textures.emplace(assetId, texture);
}

void AssetStore::AddFont(const std::string& assetId, const std::string& path, int fontSize)
{
	fonts.emplace(assetId, TTF_OpenFont(path.c_str(), fontSize));
}


//Probably would want to add this at somepoint
//void AssetStore::AddTileMap(const std::string& mapPath, const std::string& texturePath, int tileWidth, int tileHeight) {
//
//}

void AssetStore::ClearAssets() 
{
	//cannot do just this we got SDL pointers
	//textures.clear();

	for (std::pair<std::string, SDL_Texture*> texture : textures) 
	{
		SDL_DestroyTexture(texture.second);
	}

	textures.clear();

	for (std::pair<std::string, TTF_Font*> font: fonts)
	{
		TTF_CloseFont(font.second);
	}
	fonts.clear();


	//TODO: clear all sounds
	for (std::pair<std::string, Mix_Chunk*> sound : sounds)
	{
		Mix_FreeChunk(sound.second);
	}
	sounds.clear();
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) const  
{

	//Cannot use this thing if it is const for some reason
	return textures.at(assetId);

}

TTF_Font* AssetStore::GetFont(const std::string& assetId) const
{
	return fonts.at(assetId);
}

void AssetStore::AddSound(const std::string& assetId, const std::string& path)
{
	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());

	if (sound == NULL)
	{
		Logger::Err("Failed to load music track: " + path);
		return;
	}

	sounds.emplace(assetId, sound);

}

//void AssetStore::AddAnimationClip(const std::string& assetId, std::shared_ptr<AnimationClip> animationClip)
//{
//	sounds.emplace(assetId, animationClip);
//}
//void AssetStore::AddAnimationSpriteSheet(const std::string& assetId, std::shared_ptr<AnimationSpriteSheet> animationClip)
//{
//	sounds.emplace(assetId, animationClip);
//}
//
//std::shared_ptr<AnimationClip> AssetStore::GetAnimationClip(const std::string& assetId) const
//{
//	return animationClips.at(assetId);
//}
//
//std::shared_ptr<AnimationSpriteSheet> AssetStore::GetAnimationSpriteSheet(const std::string& assetId) const
//{
//	return animationSpriteSheets.at(assetId);
//}

Mix_Chunk* AssetStore::GetSound(const std::string& assetId) const
{
	return sounds.at(assetId);
}