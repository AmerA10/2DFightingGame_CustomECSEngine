#include "AssetStore.h"
#include "../logger/Logger.h"
#include <SDL_image.h>

AssetStore::AssetStore() {
	Logger::Log("Asset Store Constructor");
}

AssetStore::~AssetStore() {
	Logger::Log("Asset Store Destructor");
	
	ClearTextures();
	
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	Logger::Log("Adding Texture with name " + assetId + " path: " + filePath);

	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	//Add texture to the map
	textures.emplace(assetId, texture);
}

//Probably would want to add this at somepoint
//void AssetStore::AddTileMap(const std::string& mapPath, const std::string& texturePath, int tileWidth, int tileHeight) {
//
//}

void AssetStore::ClearTextures() {
	//cannot do just this we got SDL pointers
	//textures.clear();

	for (std::pair<std::string, SDL_Texture*> texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)  {

	//Cannot use this thing if it is const for some reason
	return textures[assetId];

}