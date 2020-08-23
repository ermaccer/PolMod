#pragma once

#define TRACK_IMAGES_START 50

struct custom_track_info {
	float fAdjustX;
	float fAdjustY;
};


class PolMod {
public:
	static bool bDebugMenu;

	static void LoadCarFiles(const char* folder);
	static void LoadTrackFiles(const char* folder);
	static void LoadCarData(char* file);
	static void LoadTrackData(char* file);
	static void LoadTrackImages();
	static void ProcessCustomInput();
	static void ProcessCustomInputInGame();
	static void DoPatches();
	// hooks
	static void HookInfo();
	static void HookCarTextName();
	static void HookTrackTextName();
	static void HookHUDData();
	static void __fastcall HookTrackPositionRace(int ptr, float x, float y, float z);
	static void __fastcall HookTrackPositionAIRace(int ptr, float x, float y, float z);
	static int __fastcall Hook41D2D0(int ptr, float rot, int unk);
	static void __fastcall HookDisplayTrackImage(int ptr, int id, float x, float y, float sizex, float sizey, int unk);
	static void HookLoadTrack();
	static void GenPathPointer();
	static void UpdatePathPointer();
	static void HookLoadPath();
	static void SetupCarRandomizer();
	static int  GetRandomizedCarID();
	static void HookCreateAICarOne();
	static void HookCreateAICarTwo();

	static void HookCreateAI2CarOne();
	static void HookCreateAI2CarTwo();

	static void HookCreateAI3CarOne();
	static void HookCreateAI3CarTwo();

};