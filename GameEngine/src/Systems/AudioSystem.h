#pragma once
#include "../ECS/ECS.h"
#include "../Components/AudioComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL_mixer.h>
#include <queue>
#include <memory>

class AudioSystem : public System
{

	//Not sure how this works yet but we would probably want
	//to keep track of which chunk is still playing
	//and probably a queue of chunks we want to play
	//So SDL has these things called channels which 
	//is just basically tracks
	//we need to manage the tracks available
	//we can use a bitset to store which channel is currently available
	//if any
	//well actually we don't need to do that


public:
	AudioSystem(int freq, int numChannels, int chunkSize)
	{
		//actually move this to the Audio System let it take care 
		//of the number of systems
		if (Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, numChannels, chunkSize) < 0)
		{
			Logger::Err("Could not init SDL Audio Mixer");
		}

		RequireComponent<AudioComponent>();
	}
	AudioSystem()
	{
		//actually move this to the Audio System let it take care 
		//of the number of systems
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 10240) < 0)
		{
			Logger::Err("Could not init SDL Audio Mixer");
		}

		RequireComponent<AudioComponent>();
	}

	void Update(const std::unique_ptr<AssetStore>& assetStore)
	{
		//All channels are busy, get out, this is pretty bad sound 
		//management but works for now I THINK

		for (auto entity : GetSystemEntities())
		{
			//first thing first get the mix_chunk 
			AudioComponent& audio = entity.GetComponent<AudioComponent>();


			if (audio.isPlaying)
			{
				continue;
			}
			if (audio.assetId.empty())
			{
				continue;
			}

			if (!audio.isMusic)
			{
				Mix_Chunk* sound = assetStore->GetSound(audio.assetId);
				//this sucks rn but lets see if it works even

				int channel = -1;
				Mix_VolumeChunk(sound, audio.volume);
				if (audio.loop)
				{
					channel = Mix_PlayChannel(-1, sound, -1);
				}
				else
				{
					channel = Mix_PlayChannel(-1, sound, 0);
				}
			}

			else 
			{
				Mix_Music* song = assetStore->GetMusic(audio.assetId);

				int channel = -1;
				Mix_VolumeMusic(audio.volume);
				if (audio.loop)
				{
					channel = Mix_PlayMusic(song, -1);
				}
				else 
				{
					channel = Mix_PlayMusic(song, 0);
				}

				Logger::Log("Playing music: " + channel);
			}

			audio.isPlaying = true;
		}
	}

	void PlaySound(Entity entity, const std::string& assetId, int volume, bool loop = false, bool isMusic = false)
	{
		AudioComponent& audio = entity.GetComponent<AudioComponent>();
		audio.assetId = assetId;
		audio.volume = volume;
		audio.loop = loop;
		audio.isMusic = isMusic;
		audio.isPlaying = false;
	}


};