#pragma once
#include <string>

enum AudioState
{
	PLAYING,
	STOPPED,
	READY
};

struct AudioComponent {
	
	std::string assetId;
	bool isPlaying;
	bool loop;
	int volume;

	//should we play this the next time we get the chance?
	bool isMusic;

	AudioComponent(const std::string assetId = "", bool loop = false, bool isMusic = false, int volume = 25)
	{
		this->assetId = assetId;
		this->loop = loop;
		this->isMusic = isMusic;
		this->volume = volume;
		this->isPlaying = false;
	}
	

};