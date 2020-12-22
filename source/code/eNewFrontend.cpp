#include "eNewFrontend.h"
#include "poldek.h"
#include "polmod.h"
#include <iostream>
#include "main.h"
#include "..\MemoryMgr.h"
#include "poldek/music.h"
using namespace Memory::VP;
using namespace PoldekDriverFunctions;

static int iTimer = GetTickCount();
float  fScrollPositionY = 0.0f;

char* szMenuNames[] = {
	"Wyúcig",
	"Trening",
	"Trasa",
	"SamochÛd",
	"Sterowanie",
	"Wyjúcie"
};

char* szMenuDifficulty[] = {
	"£atwy",
	"åredni",
	" Trudny"
};

char* szMenuCreditsOriginal[] = {
	"Programista: Mateusz Ryczko",
	"KIEROWNIK PROJEKTU: Marcin Grynkiewicz",
	"INTRO CREATOR: £ukasz Szweda",
	"GRAFIK 3D: Krzysztof £opacki",
	"GRAFIK 3D: Jakub Trojan",
	"GRAFIK 2D: Krzysztof Kostowski",
	"DèWI K: Tomek £apiÒski",
	"MUZYKA: Marcin Grynkiewicz, Adrian BartosiÒski",
	"PRODUCENT: Krzysztof Kostowski"
};

char* szMenuCreditsPolMod[] = {
	"Programista: ermaccer",

	"ArchiwaliaTV",
	"ArtKoval",
	"FranZ Gamer",
	"Frederic Mitz",
	"Gbka",
	"Gregorious",
	"GÍgarz",
	"Marcello",
	"Vapordude",
	"ciuciek",
	"robert banks",
	"åwieøy",
	" ",
	"dla polish-abandonware.pl"
};

char szTextBuffer[512];
float fCustomRotation = 1.0f;
void eNewFrontend::Init()
{
    Patch<int>(0x406F3C + 2, 0x4F2BDC); 
	Patch<int>(0x406FD4 + 2, 0x4F2BD4); 
	Patch<int>(0x406E4B + 2, 0x4F2BD9);
	Patch<int>(0x406B87 + 2, 0x4F2BD7);
	Patch<float>(0x408CF8 + 1, -900.0f);
	Patch<float>(0x408E5A + 1, 55.0f);
	Patch<float>(0x4093EC + 1, 55.0f);

	InjectHook(0x406A5B, eNewFrontend::Process, PATCH_CALL);
	InjectHook(0x411DD6, eNewFrontend::HookDrawRaceStats, PATCH_JUMP);
}

void eNewFrontend::DrawRaceStats()
{
	char textBuffer[256] = {};
	int totalCheckpoints = ((int(__thiscall*)(int))0x40A5F0)(0x4AE120);
	int ai1CheckPoints = ((int(__thiscall*)(int,int))0x40A600)(0x4AE120,0);
	int ai2CheckPoints = ((int(__thiscall*)(int,int))0x40A600)(0x4AE120,1);
	int ai3CheckPoints = ((int(__thiscall*)(int,int))0x40A600)(0x4AE120, 2);

	sprintf(textBuffer, "           %u/%u", ai1CheckPoints, totalCheckpoints);
	Draw2DText(0.80000001, 0.07, 30.0, 0.0, textBuffer);
	sprintf(textBuffer, "%s", PolMod::GetDriverName(0));
	Draw2DTextColor(0.80000001 - (strlen(PolMod::GetDriverName(0))* 0.0070f) + 0.0002f, 0.07 + 0.0005f, 38.0, 0.0, textBuffer, 0xFF000000);
	Draw2DText(0.80000001 - (strlen(PolMod::GetDriverName(0))* 0.0070f), 0.07, 38.0, 0.0, textBuffer);


	sprintf(textBuffer, "           %u/%u", ai2CheckPoints, totalCheckpoints);
	Draw2DText(0.80000001, 0.12, 30.0, 0.0, textBuffer);
	sprintf(textBuffer, "%s", PolMod::GetDriverName(1));
	Draw2DTextColor(0.80000001 - (strlen(PolMod::GetDriverName(1))* 0.0070f) + 0.0002f, 0.12 + 0.0005f, 38.0, 0.0, textBuffer, 0xFF000000);
	Draw2DText(0.80000001 - (strlen(PolMod::GetDriverName(1))* 0.0070f), 0.12, 38.0, 0.0, textBuffer);



	sprintf(textBuffer, "           %u/%u", ai3CheckPoints, totalCheckpoints);
	Draw2DText(0.80000001, 0.17, 30.0, 0.0, textBuffer);
	sprintf(textBuffer, "%s", PolMod::GetDriverName(2));
	Draw2DTextColor(0.80000001 - (strlen(PolMod::GetDriverName(2))* 0.0070f) + 0.0002f, 0.17 + 0.0005f, 38.0, 0.0, textBuffer, 0xFF000000);
	Draw2DText(0.80000001 - (strlen(PolMod::GetDriverName(2))* 0.0070f), 0.17, 38.0, 0.0, textBuffer);
}

void eNewFrontend::DrawCredits()
{
}

void eNewFrontend::ProcessMusic()
{

	if (iLastKey == 49)
		cMusicPlayer::PlayTrack("data\\music1.wav", 0); 
	if (iLastKey == 50)
		cMusicPlayer::PlayTrack("data\\music2.wav", 1);
	if (iLastKey == 51)
		cMusicPlayer::PlayTrack("data\\music3.wav", 2);
   if (iLastKey == 79)
		cMusicPlayer::StopStream();

		cMusicPlayer::Update();


}



void __fastcall eNewFrontend::Process(int ptr, float baseRotation)
{
	//begin
	BeginDraw();
	DrawImageRuntime(32, 0.0, 0.0, 64.0, 48.0, 0);
	PolMod::ProcessInfo();
	sprintf(szTextBuffer, "PolMod %s", POLMOD_VERSION);
	Draw2DTextColor(0.01f, 0.97f, 32.0, 0.0, szTextBuffer, 0xFF20E1E8);
	unsigned int ColorActive = 0xFFFFFFFF;
	unsigned int ColorIdle = 0x80FFFFFF;

	int SelectedMenu = *(int*)(ptr + 28);
	int SelectedDiff = *(int*)(ptr + 36);
	float fButtonScale = 0.0f;
	if (!SelectedMenu == MENU_QUIT)
		fScrollPositionY = 0.0f;

	for (int i = 0; i < TOTAL_MENUS; i++)
	{
		unsigned int color = 0x0;
		if (i == SelectedMenu)
		{
			color = ColorActive;
			fButtonScale = 60.0f;
		}
			
		else
		{
			color = ColorIdle;
			fButtonScale = 52.0f;
		}
			

		Draw2DTextColor(0.76000001f, 0.55499998f + 0.05 * i, fButtonScale, 0.16500001f, szMenuNames[i], color);
	}


	// logic

	switch (SelectedMenu)
	{
	case MENU_RACE:
	case MENU_PRACTICE:
		if (SelectedMenu == MENU_RACE)
		{
			for (int i = 0; i < 3; i++)
			{
				unsigned int color = 0x0;
				if (i == SelectedDiff)
					color = ColorActive;
				else
					color = ColorIdle;

				Draw2DTextColor(0.30000001f + 0.12 * i, 0.34999999f, 55.0f, 0, szMenuDifficulty[i], color);
			}
		}
		sprintf(szTextBuffer, "Auto: %s", PolMod::GetVehicleName());
		Draw2DTextColor(0.15000001f, 0.60999999f, 45.0f, 0, szTextBuffer, ColorActive);
		sprintf(szTextBuffer, "Trasa: %s", PolMod::GetTrackName());
		Draw2DTextColor(0.15000001f, 0.64999999f, 45.0f, 0, szTextBuffer, ColorActive);

		break;
	case MENU_TRACK:
		DrawImageRuntime(PoldekDriver::nSelectedTrack + 50, 0.050000001, 0.25, 40.0, 60.0, 0);
		Draw2DText(0.1, 0.73000002, 60.0, 0.0, PolMod::GetTrackName());
		break;
	case MENU_CAR:
		((void(__thiscall*)(int, float, float, float, float, float, float, float, float, float))0x40A9D0)(0x494F68, -5.0, 22.0, -80.0, 15.0, 0.0, 1.0, 0.0, 1.0, 0.0);
		((void(__thiscall*)(float))0x408C70)(baseRotation / 2);
		PolMod::ProcessCarSelectInfo();
		//Setup3DScene(0.0, 10.0, -70.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
		Draw2DText(0.10, 0.75, 45.0, 0.0, PolMod::GetVehicleName());
		sprintf(szTextBuffer, "%d/%d", PoldekDriver::nSelectedVehicle + 1, PolMod::GetVehiclesAmount());
		Draw2DText(0.10, 0.80, 70.0, 0.0, szTextBuffer);
		break;

	case MENU_CREDITS:
		Draw2DText(0.02, 0.5, 40.0, 0.0, "Sterowanie:");
		Draw2DText(0.029999999, 0.55000001, 35.0, 0.0, "G”RA - Gaz");
		Draw2DText(0.029999999, 0.60000002, 35.0, 0.0, "D”£ - Hamulec/Wsteczny");
		Draw2DText(0.029999999, 0.64999998, 35.0, 0.0, "LEWO - SkrÍt w Lewo");
		Draw2DText(0.029999999, 0.69999999, 35.0, 0.0, "PRAWO - SkrÍt w Prawo");
		Draw2DText(0.029999999, 0.75, 35.0, 0.0, "SPACJA - Hamulec RÍczny");
		Draw2DText(0.029999999, 0.80000001, 35.0, 0.0, "Lewy Control - Palenie Gum");
		Draw2DText(0.41999999, 0.5, 40.0, 0.0, "Kamera:");
		Draw2DText(0.43000001, 0.55000001, 35.0, 0.0, "C - zmiana Kamery");
		Draw2DText(0.43000001, 0.60000002, 35.0, 0.0, "< - ObrÛt W lewo");
		Draw2DText(0.43000001, 0.64999998, 35.0, 0.0, "> - ObrÛt W prawo");
		Draw2DText(0.43000001, 0.69999999, 35.0, 0.0, "PGUP - ruch w gÛrÍ");
		Draw2DText(0.43000001, 0.75, 35.0, 0.0, "PGDN - ruch w dÛ≥");
		Draw2DText(0.43000001, 0.80000001, 35.0, 0.0, "V - przywracanie kamery");
		Draw2DText(0.12, 0.89999998, 35.0, 0.0, "M - Lusterko");
		Draw2DText(0.43000001, 0.89999998, 35.0, 0.0, "O, 1, 2, 3 - Muzyka");
		break;
	case MENU_QUIT:
		// original

		if (GetTickCount() - iTimer <= 1) return;
		iTimer = GetTickCount();
		

		Draw2DText(0.02f, 0.35f + +fScrollPositionY, 35.0, 0.0, "TwÛrcy gry:" );
		for (int i = 0; i < sizeof(szMenuCreditsOriginal) / sizeof(szMenuCreditsOriginal[0]); i++)
		{
			Draw2DText(0.02f,0.4f + (0.05f * i) + fScrollPositionY, 40.0,0.0, szMenuCreditsOriginal[i]);
		}
		Draw2DText(0.02f, 0.85f + +fScrollPositionY, 35.0, 0.0, "PolMod:");
		for (int i = 0; i < sizeof(szMenuCreditsPolMod) / sizeof(szMenuCreditsPolMod[0]); i++)
		{
			Draw2DText(0.02f, 0.90f + (0.05f * i) + +fScrollPositionY, 40.0, 0.0, szMenuCreditsPolMod[i]);
		}

		fScrollPositionY -= 0.0005f;

		break;
	}

	//end
	EndDraw();
}

void __fastcall eNewFrontend::ProcessLogic(int ptr, float unk)
{
	//ProcessMusic();
	Process(ptr, unk);

	int& SelectedMenu = *(int*)(ptr + 28);
	printf("%x (%d)\n", iLastKey, SelectedMenu);

	if (iLastKey == VK_UP)
		SelectedMenu--;

	if (iLastKey == VK_DOWN)
		SelectedMenu++;

	if (SelectedMenu < 0) SelectedMenu = TOTAL_MENUS;
	if (SelectedMenu > TOTAL_MENUS) SelectedMenu = 0;
}

void __declspec(naked) eNewFrontend::HookDrawRaceStats()
{
	DrawRaceStats();

	static int jmpPoint = 0x411EE8;
	_asm jmp jmpPoint
}
