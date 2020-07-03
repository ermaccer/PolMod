#pragma once


namespace PolMod {
	void LoadCarFiles(const char* folder);
	void LoadCarData(char* file);
	void ProcessCustomInput();
	void ProcessCustomInputInGame();
	void DoPatches();
	// hooks
	void HookInfo();
	void HookCarTextName();
	void HookHUDData();
}