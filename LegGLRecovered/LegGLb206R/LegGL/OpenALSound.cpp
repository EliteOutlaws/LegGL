#include "OpenALSound.h"
#include "LegFrameworkClass.h"
extern void DebugLog(char* Message);
extern LegFrameworkClass p_lfc;

#define BUFFER_SIZE 15000

struct legSoundCleanupStruct
{
	ALuint SourceIDCopy;
	ALuint BufferIDCopy;
}l_scs;
int legSoundCleanupThread(LPVOID Param)
{
	legSoundCleanupStruct StructCopy;
	memcpy(&StructCopy, Param, (sizeof legSoundCleanupStruct));
	ALint state;
    do
        {
        alGetSourcei(StructCopy.SourceIDCopy, AL_SOURCE_STATE, &state);
		Sleep(100);
        }
    while (state != AL_STOPPED);

    // Clean up sound buffer and source
    alDeleteBuffers(1, &StructCopy.BufferIDCopy);
    alDeleteSources(1, &StructCopy.SourceIDCopy);

	return 0;
}

void LegSoundscape::legLoadWavFile(char *fileName)
{
    FILE *fp = NULL;                                                
    fp=fopen(fileName,"rb");                                        
    if (!fp) DebugLog("Sound error - Could not open sound file");   
    
    
    char type[4];
    DWORD size,chunkSize;
    short formatType,channels;
    DWORD avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    
    //Check that the WAVE file is OK
    fread(type,sizeof(char),4,fp);                                  

    fread(&size, sizeof(DWORD),1,fp);                               
    fread(type, sizeof(char),4,fp);                                 
    
    fread(type,sizeof(char),4,fp);                                  
    
    //Now we know that the file is a acceptable WAVE file
    //Info about the WAVE data is now read and stored
    fread(&chunkSize,sizeof(DWORD),1,fp);
    fread(&formatType,sizeof(short),1,fp);
    fread(&channels,sizeof(short),1,fp);
    fread(&sampleRate,sizeof(DWORD),1,fp);
    fread(&avgBytesPerSec,sizeof(DWORD),1,fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    fread(&bitsPerSample,sizeof(short),1,fp);
    
    fread(type,sizeof(char),4,fp);                                  
    
    fread(&dataSize,sizeof(DWORD),1,fp);                            
    
        
    buf = new unsigned char[dataSize];                            
    fread(buf,sizeof(BYTE),dataSize,fp);

	if(bitsPerSample == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }

}

void LegSoundscape::legPrepareWavInBuffer(void)
{                             
ALuint frequency=sampleRate;                                                                                                         
alGenBuffers(1, &buffer);                                                   
alGenSources(1, &source);                                                   
alBufferData(buffer, format, buf, dataSize, frequency);
}

int LegSoundscape::playMusic(char* fileName, float Pitch)
{
legLoadWavFile(fileName);
legPrepareWavInBuffer();

	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 
                                                                                
    //Listener                                                                  
    alListenerfv(AL_POSITION,    ListenerPos);                                  
    alListenerfv(AL_VELOCITY,    ListenerVel);                                  
    alListenerfv(AL_ORIENTATION, ListenerOri);                                  
    
    //Source
    alSourcei (source, AL_BUFFER, buffer);                           
    alSourcef (source, AL_PITCH, Pitch);                              
    alSourcef (source, AL_GAIN, Gain);                               
    alSourcefv(source, AL_POSITION, SourcePos);                      
    alSourcefv(source, AL_VELOCITY, SourceVel);                      
    alSourcei (source, AL_LOOPING,  AL_FALSE);                       
    
    //PLAY 
    alSourcePlay(source);                                            
    //if(alGetError() != AL_NO_ERROR) DebugLog("Error playing sound"); 
  
	l_scs.BufferIDCopy = buffer;
	l_scs.SourceIDCopy = source;
    CreateThread(0,0,(LPTHREAD_START_ROUTINE)legSoundCleanupThread, &l_scs, 0, 0);

	return true;
}
static int _ov_header_fseek_wrap(FILE *f,ogg_int64_t off,int whence)
{
  if(f==NULL)return(-1);
}
static ov_callbacks OV_CALLBACKS_DEFAULT = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
  (int (*)(void *))                             fclose,
  (long (*)(void *))                            ftell
};

void LegSoundscape::legLoadOGGFile(char *fileName, std::vector<char> &buffer, ALenum &format2, ALsizei &freq)
    {
    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];                // Local fixed size array
    FILE *f;

    // Open for binary reading
    f = fopen(fileName, "rb");

	if (f == NULL) DebugLog("Sound error - Failed to open OGG file! (#1)");
    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open_callbacks(f, &oggFile, NULL, 0, OV_CALLBACKS_DEFAULT) != 0) 
		DebugLog("Sound error - Failed to open OGG file! (#2)");

    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    freq = pInfo->rate;

    // Keep reading until all is read
    do
        {
 
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

		if (bytes < 0) DebugLog("Sound error - Failed to decode ogg file! (#1)");
    

        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
        }
    while (bytes > 0);

    // Clean up!
    ov_clear(&oggFile);
    }


int LegSoundscape::playOggMusic(char* fileName, float Pitch)
    {                       
    ALuint bufferID;                       
    ALuint sourceID;                       
    ALenum format2;                         
    ALsizei freq;                          
    std::vector<char> bufferData;               

    // Create sound buffer and source
    alGenBuffers(1, &bufferID);
    alGenSources(1, &sourceID);

    // Set the source and listener to the same location
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);                          
    alSourcef (sourceID, AL_PITCH, Pitch);                              
    alSourcef (sourceID, AL_GAIN, Gain);                                                    
    alSourcei (sourceID, AL_LOOPING,  AL_FALSE);          
	g_OGGSourceID = sourceID;

    // Load the OGG file into memory
    legLoadOGGFile(fileName, bufferData, format2, freq);

    // Upload sound data to buffer
    alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);

    // Attach sound buffer to source
    alSourcei(sourceID, AL_BUFFER, bufferID);

    // Finally, play the sound!!!
    alSourcePlay(sourceID);

	l_scs.BufferIDCopy = bufferID;
	l_scs.SourceIDCopy = sourceID;
    CreateThread(0,0,(LPTHREAD_START_ROUTINE)legSoundCleanupThread, &l_scs, 0, 0);

    return 0;
    }

bool LegSoundscape::OGGMusicIsPlaying()
{
	int state;
	alGetSourcei(g_OGGSourceID, AL_SOURCE_STATE, &state);
	if(state==AL_PLAYING) return true;
	else return false;
}

int LegSoundscape::InitSound()
{

// Init OpenAL
ALCcontext *Context;
ALCdevice *Device;


Device = alcOpenDevice(0);
//Device = alcOpenDevice((ALCchar*)"DirectSound3D");
if (!Device) 
{
	DebugLog("Failed to initialize DirectSound3D");
	exit(-1);
}
// Create and set context
Context=alcCreateContext(Device,NULL);
alcMakeContextCurrent(Context);
alGetError();
  return true;
}