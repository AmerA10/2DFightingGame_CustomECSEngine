#pragma once
#include <string>

struct AudioComponent {
	
	std::string assetId;
	bool isPlaying;
	bool loop;
	int volume;

	//should we play this the next time we get the chance?
	bool play;

	AudioComponent(const std::string assetId = "", bool loop = false, bool play = false, int volume = 25)
	{
		this->assetId = assetId;
		this->loop = loop;
		this->play = play;
		this->volume = volume;
	}
	

};