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
	void AddSpriteSheet(const std::string& assetId, SpriteSheet sheet);
	void AddFont(const std::string& assetId, const std::string& path, int fontSize);
	void AddSound(const std::string& assetId, const std::string& path);
	void AddAnimationClip(const std::string& assetId, std::shared_ptr<SpriteAnimationClip> animClip);
	void AddFAnimationClip(const std::string& assetId, FAnimationClip animClip);
	void AddMusic(const std::string& assetId, const std::string& path);


	SDL_Texture* GetTexture(const std::string& assetId) const;
	TTF_Font* GetFont(const std::string& assetId) const;
	Mix_Chunk* GetSound(const std::string& assetId) const;
	Mix_Music* GetMusic(const std::string& assetId)const;

	//this needs fixing as well
	std::shared_ptr<SpriteAnimationClip> GetAnimationClip(const std::string& assetId) const;
	std::unique_ptr<FAnimationClip>& GetFAnimationClip(const std::string& assetId);
	std::unique_ptr<SpriteSheet>& GetSpriteSheet(const std::string& assetId);


	AssetStore();
	~AssetStore();


private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> sounds;
	std::map<std::string, Mix_Music*> songs;

	//TODO:
	//change this to a unqiue_ptr and pass it by reference, we do not
	//need multiplle ownership because most of this is reading
	std::map <std::string, std::shared_ptr<SpriteAnimationClip>> animationClips;
	std::map<std::string, std::unique_ptr<FAnimationClip>> fAnimationClips;
	std::map<std::string, std::unique_ptr<SpriteSheet>> spriteSheets;
	

};





#endif