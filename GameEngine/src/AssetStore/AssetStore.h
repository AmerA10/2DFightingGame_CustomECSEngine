#pragma once
#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

//This class is responsible for storing and managing our assets

class AssetStore {
public:
	
	void ClearAssets();

	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
	void AddFont(const std::string& assetId, const std::string& path, int fontSize);

	SDL_Texture* GetTexture(const std::string& assetId) const;
	TTF_Font* GetFont(const std::string& assetId) const;

	AssetStore();
	~AssetStore();


private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	
	

};





#endif