// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "mr.h"
#include <time.h>


using namespace Memory::VP;



void Init()
{
	LoadCarFiles("CARS");
//	LoadTrackFiles("TRACKS");

	// disable start movie
	Nop(0x41C00F, 5);
	// disable start adverts (duh)
	Nop(0x41C8E3, 5);
	Nop(0x41C8FE, 5);

	// disable "game" logo
	Patch<int>(0x41C376 + 1, -1);
	Patch<int>(0x41C38B + 1, -1);

	Patch<int>(0x401C38 + 6, 5);

	DoPatches();
	Patch<int>(0x411BF6 + 1, (int)HookHUDData - ((int)0x411BF6 + 5));


	InjectHook(0x40777F, HookCarTextName, PATCH_JUMP);
	//InjectHook(0x4075C8, HookTrackTextName, PATCH_JUMP);
	InjectHook(0x40709B, HookInfo, PATCH_JUMP);
	InjectHook(0x41ACF6, HookCarAmount, PATCH_JUMP);


}

extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}