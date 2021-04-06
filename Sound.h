#pragma once

#include <xaudio2.h>

class Sound
{
public:
	Sound(const char* filename, bool loop);
	~Sound();

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	void Play();
	void Stop();

private:
	IXAudio2 *xaudio2;
	IXAudio2MasteringVoice *masterVoice;

	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	IXAudio2SourceVoice *sourceVoice;
};