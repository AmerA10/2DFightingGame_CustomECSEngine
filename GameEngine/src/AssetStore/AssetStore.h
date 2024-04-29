#pragma once
#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <map>
#include <string>
#include <SDL.h>

//This class is responsible for storing and managing our assets

class AssetStore {
public:
	
	void ClearTextures();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
	SDL_Texture* GetTexture(const std::string& assetId) ;

	AssetStore();
	~AssetStore();


private:
	std::map<std::string, SDL_Texture*> textures;
	
	

};





#endif