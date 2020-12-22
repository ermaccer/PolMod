// dllmain.cpp : Defines the entry point for the DLL application.

#include <iostream>
#include "pch.h"
#include "MemoryMgr.h"
#include "code/polmod.h"
#include "code/poldekmp.h"
#include "code/main.h"
#include "dllmain.h"
#include "code/poldekmp.h"
#include "code/eNewFrontend.h"

#include "code/eSettingsManager.h"

using namespace Memory::VP;
using namespace MP;

void null() {}


void Init()
{
	SettingsMgr->Init();

	if (SettingsMgr->bWlaczKonsole)
	{
		AllocConsole();
	    freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}

	PolMod::SetupRandomizer();
	PolMod::SetupDriverNames();
	PolMod::LoadCarFiles("CARS");

	PolMod::LoadTrackFiles("TRACKS");
	PolMod::GenPathPointer();
	PolMod::DoPatches();
	//intro
	if (SettingsMgr->bWylaczIntro)
	Nop(0x41C458, 5);
    // reklamy
	Nop(0x41CD2C, 5);
	Nop(0x41CD47, 5);
	// logo
	Patch<int>(0x41C7BF + 1, -1);
	Patch<int>(0x41C7D4 + 1, -1);

	Nop(0x419426, 4);
	Nop(0x419451, 4);
	Nop(0x419476, 4);

	InjectHook(0x40709B, PolMod::HookInfo, PATCH_JUMP);
	InjectHook(0x407794, PolMod::HookCarTextName, PATCH_JUMP);
	InjectHook(0x4075C8, PolMod::HookTrackTextName, PATCH_JUMP);
	InjectHook(0x411C16, PolMod::HookHUDData, PATCH_CALL);

	InjectHook(0x407FA5, PolMod::HookTrackPositionRace, PATCH_CALL);
	InjectHook(0x408315, PolMod::HookTrackPositionRace, PATCH_CALL);
	InjectHook(0x4088FA, PolMod::HookTrackPositionRace, PATCH_CALL);
	InjectHook(0x408B64, PolMod::HookTrackPositionRace, PATCH_CALL);


	InjectHook(0x4080D6, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	InjectHook(0x4081A7, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	InjectHook(0x408279, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	InjectHook(0x4089C2, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	InjectHook(0x408A63, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	InjectHook(0x408B07, PolMod::HookTrackPositionAIRace, PATCH_CALL);
	Patch<char>(0x484504, 0x00);


    Nop(0x407FE1, 8);
	InjectHook(0x407FE1, PolMod::HookLoadPath, PATCH_JUMP);
	
	InjectHook(0x40805A, PolMod::HookCreateAICarOne, PATCH_JUMP);
	InjectHook(0x408072, PolMod::HookCreateAICarTwo, PATCH_JUMP);

	InjectHook(0x408125, PolMod::HookCreateAI2CarOne, PATCH_JUMP);
	InjectHook(0x408143, PolMod::HookCreateAI2CarTwo, PATCH_JUMP);

	InjectHook(0x4081F8, PolMod::HookCreateAI3CarOne, PATCH_JUMP);
	InjectHook(0x408215, PolMod::HookCreateAI3CarTwo, PATCH_JUMP);


	if (SettingsMgr->bEnableMP)
	{
		MP::InitHooks();
		Nop(0x40837D, 7);
		InjectHook(0x40837D, PolMod::HookLoadTrack, PATCH_JUMP);
	}

	Patch<int>(0x41C46E + 4, (int)MainHooks::HookedWndProc);
	eNewFrontend::Init();

}


extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}

