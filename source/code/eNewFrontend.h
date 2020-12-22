#pragma once

enum eFrontend {
	MENU_RACE,
	MENU_PRACTICE,
	MENU_TRACK,
	MENU_CAR,
	MENU_CREDITS,
	MENU_QUIT,
	TOTAL_MENUS
};

class eNewFrontend {

public:
	int nSelectedMenu;
	int iCursorX;
	int iCursorY;

	static void Init();
	static void DrawRaceStats();
	static void DrawCredits();
	static void ProcessMusic();


	static void __fastcall Process(int ptr, float baseRotation);
	static void __fastcall ProcessLogic(int ptr, float unk);


	static void HookDrawRaceStats();
};