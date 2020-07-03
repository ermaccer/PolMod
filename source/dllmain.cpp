// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "code/polmod.h"

using namespace Memory::VP;

void Init()
{
	PolMod::LoadCarFiles("CARS");
	PolMod::DoPatches();
	//intro
	Nop(0x41C458, 5);
    // reklamy
	Nop(0x41CD2C, 5);
	Nop(0x41CD47, 5);
	// logo
	Patch<int>(0x41C7BF + 1, -1);
	Patch<int>(0x41C7D4 + 1, -1);

	InjectHook(0x40709B, PolMod::HookInfo, PATCH_JUMP);
	InjectHook(0x407794, PolMod::HookCarTextName, PATCH_JUMP);
	InjectHook(0x411C16, PolMod::HookHUDData, PATCH_CALL);
}


extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}