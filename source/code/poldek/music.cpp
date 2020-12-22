#include "music.h"
#include "core.h"

void cMusicPlayer::CloseStream()
{
	Call<0x41D110>();
}

void cMusicPlayer::LoadStream(const char * path)
{
	Call<0x41D060, const char*>(path);
}

void cMusicPlayer::Begin()
{
	Call<0x41D0D0>();
}

void cMusicPlayer::StopStream()
{
	*(int*)0x492800 = 0;
}

void cMusicPlayer::PlayTrack(const char * path, int id)
{
	CloseStream();
	LoadStream(path);
	if (*(int*)0x494E3C)
	{
		Begin();
		*(int*)0x494E40 = id;
	}
}

void cMusicPlayer::Update()
{
	if (!*(int*)0x492800)
	 {
	  bool isPlaying = *(int*)0x494E3C == 0;
	  *(int*)0x494E3C = isPlaying;
	  if (isPlaying)
		  Begin();
	  else
		  Call<0x41D0F0>();
	  *(int*)0x492800 = 1;
	 }
}
