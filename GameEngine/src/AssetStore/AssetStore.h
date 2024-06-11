#pragma once
#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../Animation/AnimationClip.h"
#include <memory>

//This class is responsible for storing and managing our assets

class AssetStore {
public:
	
	void ClearAssets();

	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
	void AddFont(const std::string& assetId, const std::string& path, int fontSize);
	void AddSound(const std::string& assetId, const std::string& path);
	//void AddAnimationClip(const std::string& assetId, std::shared_ptr<AnimationClip> animClip);
	//void AddAnimationSpriteSheet(const std::string& assetId, std::shared_ptr<AnimationSpriteSheet> animSheet);


	SDL_Texture* GetTexture(const std::string& assetId) const;
	TTF_Font* GetFont(const std::string& assetId) const;
	Mix_Chunk* GetSound(const std::string& assetId) const;
	//std::shared_ptr<AnimationClip> GetAnimationClip(const std::string& assetId) const;
	//std::shared_ptr<AnimationSpriteSheet> GetAnimationSpriteSheet(const std::string& assetId) const;


	AssetStore();
	~AssetStore();


private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> sounds;
	//std::map < std::string, std::shared_ptr<AnimationClip>> animationClips;
	//std::map < std::string, std::shared_ptr<AnimationSpriteSheet>> animationSpriteSheets;
	

};





#endif