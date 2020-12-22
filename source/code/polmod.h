#pragma once
#include "poldek.h"
#include "polmod.h"
#include "poldekmp.h"

#define TRACK_IMAGES_START 50

struct custom_track_info {
	float fAdjustX;
	float fAdjustY;
};


class PolMod {
public:
	static bool bDebugMenu;
	static bool bHideHud;

	static void LoadCarFiles(const char* folder);
	static void LoadTrackFiles(const char* folder);
	static void LoadCarData(char* file);
	static void LoadTrackData(char* file);
	static void LoadTrackImages();
	static void ProcessCustomInput();
	static void ProcessCustomInputInGame();
	static void DoPatches();
	static void ProcessInfo();
	static void ProcessCarSelectInfo();
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
	static void SetupRandomizer();
	static void SetupDriverNames();
	static void HookDriverName(char* dest, char* text, int min, int max);
	static void HookDriverName2(char* dest, char* text, int min, int max);
	static void HookDriverName3(char* dest, char* text, int min, int max);

	static char* GetDriverName(int id);

	static int  GetRandomizedCarID();

	static char* GetVehicleCar();
	static char* GetVehicleName();
	static char* GetVehicleMar();
	static int   GetVehiclesAmount();
	static char* GetTrackName();

	static vector GetTrackPos(ePlayerIDs who);

	static void HookCreateAICarOne();
	static void HookCreateAICarTwo();

	static void HookCreateAI2CarOne();
	static void HookCreateAI2CarTwo();

	static void HookCreateAI3CarOne();
	static void HookCreateAI3CarTwo();

};