#pragma once

class cMusicPlayer {
public:
	static void CloseStream();
	static void LoadStream(const char* path);
	static void Begin();
	static void StopStream();

	static void PlayTrack(const char* path, int id);
	static void Update();

};