#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
#include "AL\al.h"
#include "AL\alc.h"
#include "Sound/vorbis/vorbisfile.h"
#include "cCVARS.h"
#pragma comment (lib, "OpenAL32.lib")
#pragma comment (lib, "libvorbis.lib")
#pragma comment (lib, "libvorbisfile.lib")
#pragma comment (lib, "libogg.lib")

class LegSoundscape
{
public:
	// Initialize soundscape
	int InitSound();

	// Play a file that can always be heard
	int playMusic(char* fileName, float Pitch);
	int playOggMusic(char* fileName, float Pitch);
	
	// Retrieve values about a file to set it up
	void legLoadWavFile(char *fileName);
	void legLoadOGGFile(char *fileName, std::vector<char> &buffer, ALenum &format2, ALsizei &freq);

	// Call this after loading a file up into memory
	void legPrepareWavInBuffer(void);

	// Check if a music file is already running
	int g_OGGSourceID;
	bool OGGMusicIsPlaying();


	// Public wav values
	ALenum format;
	void *data; 
	ALsizei size; 
	ALsizei frequency;
	ALboolean loop;
	DWORD sampleRate;
	DWORD dataSize;
	unsigned char* buf;
	ALuint source;                                                              
	ALuint buffer; 

	// Cvars
	float Gain;
};