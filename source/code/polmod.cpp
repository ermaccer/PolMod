#include "..\pch.h"
#include "poldek.h"
#include "polmod.h"
#include <vector>
#include <string>
#include <filesystem>
#include "..\IniReader.h"
#include "..\MemoryMgr.h"
#include <time.h>
#include "poldekmp.h"
#include <memory>
#include "eSettingsManager.h"

using namespace Memory::VP;
using namespace PoldekDriverFunctions;

int tmp_ecx = 0;

char strTemp[256];
char tmp[256];
int  pStringPointer = 0;
int  pPathPointer = 0;
int  pTrackPointer = 0;

int  pCarPointer1 = 0;
int  pCarPointer2 = 0;
int  pCarPointer3 = 0;
int  pCarMarPointer1 = 0;
int  pCarMarPointer2 = 0;
int  pCarMarPointer3 = 0;
int nLastRandomNumber = 0;


bool bEnableSelMenu = 0;

int nTempSelection = 0;
static int keyTimer = GetTickCount();



struct path {
	char name[64];
};


std::vector<car_info> carData;
std::vector<car_info> ai_carData;
std::vector<custom_car_info> customCarData;
std::vector<custom_car_info> ai_customCarData;

std::vector<track_info> trackData;
std::vector<std::string> trackPaths;
std::unique_ptr<path[]> pathData;
std::vector<int> trackSecondVariant;
std::vector<int> trackSecondVariantNo;
std::vector<custom_track_info> customTrackData;
std::vector<std::string> carNames;
std::vector<std::string> trackNames;
std::vector<std::string> carFilePaths;
std::vector<std::string> trackFilePaths;



bool PolMod::bDebugMenu;


void PolMod::LoadCarFiles(const char * folder)
{
	if (!std::experimental::filesystem::exists(folder))
	{
		MessageBoxA(0, "Folder does not exist!", folder, 0);
	}
	else
	{
		for (const auto & file : std::experimental::filesystem::recursive_directory_iterator(folder))
		{
			if (file.path().has_extension())
			{
				if (file.path().extension().string() == ".ini")
				{
					carFilePaths.push_back(file.path().string());
				}
			}
		}

		for (int i = 0; i < carFilePaths.size(); i++)
			LoadCarData((char*)carFilePaths[i].c_str());
	}
}

void PolMod::LoadTrackFiles(const char * folder)
{
	if (!std::experimental::filesystem::exists(folder))
	{
		MessageBoxA(0, "Folder does not exist!", folder, 0);
	}
	else
	{
		for (const auto & file : std::experimental::filesystem::recursive_directory_iterator(folder))
		{
			if (file.path().has_extension())
			{
				if (file.path().extension().string() == ".ini")
				{
					trackFilePaths.push_back(file.path().string());
				}
			}
		}

		for (int i = 0; i < trackFilePaths.size(); i++)
			LoadTrackData((char*)trackFilePaths[i].c_str());
	}
}

void PolMod::LoadCarData(char * file)
{
	CIniReader ini(file);
	car_info car;
	custom_car_info cc;
	sprintf(car.marArchive, ini.ReadString("Car", "MarArchive", 0));
	sprintf(car.carFile, ini.ReadString("Car", "CarFile", 0));
	car.unlocked = ini.ReadInteger("Car", "Unlocked", 1);	
	car.unknownFloat = ini.ReadFloat("Car", "UnknownFloat", 0.0f);
	cc.noCollisionData = ini.ReadBoolean("Car", "NoDamagedParts", true);
	cc.mrGear = ini.ReadBoolean("Car", "Use4Gears", false);


	cc.gearShift[0] = ini.ReadFloat("GearShift", "1", 1.0f);
	cc.gearShift[1] = ini.ReadFloat("GearShift", "2", 30.0f);
	cc.gearShift[2] = ini.ReadFloat("GearShift", "3", 50.0f);
	cc.gearShift[3] = ini.ReadFloat("GearShift", "4", 80.0f);
	cc.gearShift[4] = ini.ReadFloat("GearShift", "5", 120.0f);

	bool ai = ini.ReadBoolean("Car", "AI", false);


	char temp[256];
	sprintf(temp, "%s", ini.ReadString("Car", "Name", "No name"));
	std::string str(temp, strlen(temp));


	printf("Wczytywanie pojazdu %s (%s)\n", car.carFile, str.c_str());

	if (ai) {
		ai_carData.push_back(car);
		ai_customCarData.push_back(cc);
	}
	else {
		carData.push_back(car);
		customCarData.push_back(cc);
		carNames.push_back(str);
	}


}

void PolMod::LoadTrackData(char * file)
{
	CIniReader ini(file);
	track_info info;
	custom_track_info custom_track;
	path extra;

	sprintf(info.marArchive, ini.ReadString("Track", "MAR", 0));
	sprintf(info.trackFile, ini.ReadString("Track", "TRK", 0));


	char path[256];
	sprintf(path, "%s", ini.ReadString("Track", "PTH", 0));
	std::string pth(path, strlen(path));
	trackPaths.push_back(pth);


	custom_track.fAdjustX = ini.ReadFloat("Track", "AdjustX", 50.0f);
	custom_track.fAdjustY = ini.ReadFloat("Track", "AdjustY", 30.0f);

	//sprintf(extra.name, ini.ReadString("Track", "PTH", 0));

	info.unlock = ini.ReadInteger("Car", "Unlocked", 1);

	info.x = ini.ReadFloat("Track", "X", 1.0f);
	info.y = ini.ReadFloat("Track", "Y", 1.0f);
	info.r = ini.ReadFloat("Track", "R", 1.0f);



	char temp[256];
	sprintf(temp, "%s", ini.ReadString("Track", "Name", "No name"));
	std::string str(temp, strlen(temp));


	printf("Wczytywanie trasy %s (%s)\n", info.trackFile, str.c_str());

	int var = 0;
	trackSecondVariant.push_back(var);
	int no = 1;
	trackSecondVariantNo.push_back(no);
	trackNames.push_back(str);

	trackData.push_back(info);
	customTrackData.push_back(custom_track);

}

void PolMod::LoadTrackImages()
{
	for (int i = 0; i < trackData.size(); i++)
	{
	}
}

void PolMod::ProcessCustomInput()
{
	if (GetAsyncKeyState(86))
	{
		if (GetTickCount() - keyTimer <= 150) return;
		keyTimer = GetTickCount();
		bEnableSelMenu ^= 1;
	}


	if (nTempSelection + 1 > carData.size()) nTempSelection = 0;
	if (nTempSelection < 0) nTempSelection = carData.size() - 1;

	if (bEnableSelMenu)
	{
		if (GetAsyncKeyState(90))
		{
			if (GetTickCount() - keyTimer <= 150) return;
			keyTimer = GetTickCount();
			nTempSelection++;
		}
		if (GetAsyncKeyState(88))
		{
			if (GetTickCount() - keyTimer <= 150) return;
			keyTimer = GetTickCount();
			nTempSelection--;
		}
		if (GetAsyncKeyState(67))
			PoldekDriver::nSelectedVehicle = nTempSelection;

	}
}

void PolMod::ProcessCustomInputInGame()
{

}

void PolMod::DoPatches()
{

	// liczba
	int cars = carData.size();
	Patch<int>(0x41AD25 + 6, cars);

	int tracks = trackData.size();
	Patch<int>(0x41AD2F + 6, tracks);

	// .car
	Patch<int>(0x407F4B + 3, (int)&carData[0]);
	Patch<int>(0x4082C7 + 3, (int)&carData[0]);

	// .mar
	Patch<int>(0x4082A9 + 3, (int)&carData[0] + 64);
	Patch<int>(0x407F2D + 3, (int)&carData[0] + 64);


	// odblokowanie
	int unlockAdresses[] = { 0x6A21,
		0x6F79,0x7008,0x7813,0x1A55B,0x1C35C };

	for (int i = 0; i < sizeof(unlockAdresses) / sizeof(int); i++)
		Patch<int>(0x400000 + unlockAdresses[i], (int)&carData[0].unlocked);

	// .trk
	Patch<int>(0x40837D + 3, (int)&trackData[0]);
	Patch<int>(0x408360 + 3, (int)&trackData[0].marArchive);

	Patch<int>(0x407E3D + 1, (int)&trackData[0].marArchive);

	/*
	Patch<int>(0x407F8B + 2, (int)&trackData[0].r);
	Patch<int>(0x40803D + 3, (int)&trackData[0].r);
	Patch<int>(0x4080B7 + 2, (int)&trackData[0].r);
	Patch<int>(0x408182 + 2, (int)&trackData[0].r);
	Patch<int>(0x4081E1 + 3, (int)&trackData[0].r);
	Patch<int>(0x40825A + 2, (int)&trackData[0].r);
	Patch<int>(0x4088E0 + 2, (int)&trackData[0].r);
	Patch<int>(0x408954 + 3, (int)&trackData[0].r);
	Patch<int>(0x4089A3 + 2, (int)&trackData[0].r);
	Patch<int>(0x4089F7 + 2, (int)&trackData[0].r);
	Patch<int>(0x408A44 + 2, (int)&trackData[0].r);
	Patch<int>(0x408A99 + 3, (int)&trackData[0].r);
	Patch<int>(0x408AE8 + 2, (int)&trackData[0].r);
	
	Patch<int>(0x407F98 + 2, (int)&trackData[0].x);
	Patch<int>(0x4080C5 + 2, (int)&trackData[0].x);
	Patch<int>(0x408196 + 2, (int)&trackData[0].x);
	Patch<int>(0x408268 + 2, (int)&trackData[0].x);
	Patch<int>(0x4088ED + 2, (int)&trackData[0].x);
	Patch<int>(0x4089B1 + 2, (int)&trackData[0].x);
	Patch<int>(0x408A52 + 2, (int)&trackData[0].x);
	Patch<int>(0x408AF6 + 2, (int)&trackData[0].x);

	Patch<int>(0x407F91 + 2, (int)&trackData[0].y);
	Patch<int>(0x4080B1 + 2, (int)&trackData[0].y);
	Patch<int>(0x408188 + 2, (int)&trackData[0].y);
	Patch<int>(0x408254 + 2, (int)&trackData[0].y);
	Patch<int>(0x4088E6 + 2, (int)&trackData[0].y);
	Patch<int>(0x40899D + 2, (int)&trackData[0].y);
	Patch<int>(0x408A3E + 2, (int)&trackData[0].y);
	Patch<int>(0x408AE2 + 2, (int)&trackData[0].y);*/




	int osAddresses[] = {
		0x6DF2,0x6E03,0x6E0F,0x6EDA,0x6EF5,0x6EFC,0x754E,0x7F79,0x7FCD,
		0x802A,0x809F,0x80FD, 0x8170,0x81CE,0x8242,0x88CE,0x8941,0x898B,
		0x89E4,0x8A2C,0x8A86,0x8AD0,0x1A2F7,0x1A33A,0x1A385,0x1A3B7,0x1A3F7,
		0x1A43A,0x1A47E,0x1A4CE,0x1A50D,0x1ACD3
	};

	for (int i = 0; i < sizeof(osAddresses) / sizeof(int); i++)
		Patch<int>(0x400000 + osAddresses[i], (int)&trackSecondVariant[0]);


	int osNoAddresses[] = {
		0x6ED3,0x6EEE
	};

	for (int i = 0; i < sizeof(osNoAddresses) / sizeof(int); i++)
		Patch<int>(0x400000 + osNoAddresses[i], (int)&trackSecondVariantNo[0]);

	//Patch<int>(0x407FE1 + 3, (int)&trackExtra[0].name);


	// jakis float
	Patch<int>(0x408008 + 3, (int)&carData[0].unknownFloat);

	// rozmiar tekstu aut
	Patch<float>(0x4077EB + 1, 40.0f);


}

void __declspec(naked)  PolMod::HookInfo()
{
	static int jmpContinue = 0x4070A0;

	

	_asm {
		mov eax, 0x40C810
		call eax
	}


	// kamera z tylu
	PoldekDriver::nCurrentCamera = CAMERA_REAR;

	if (customCarData[PoldekDriver::nSelectedVehicle].mrGear)
		Patch<int>(0x401C38 + 6, 4);
	else
		Patch<int>(0x401C38 + 6, 5);

	if (customCarData[PoldekDriver::nSelectedVehicle].noCollisionData)
	{
		Nop(0x405C6C, 4);
		Nop(0x405C90, 4);
		Nop(0x405CB4, 4);
	}
	else
	{
		Patch<int>(0x405C6C, 0x37DC085);
		Patch<int>(0x405C90, 0x37DC085);
		Patch<int>(0x405CB4, 0x37DC085);
	}

	Patch<float>(0x4742D8, customCarData[PoldekDriver::nSelectedVehicle].gearShift[0]);
	Patch<float>(0x4742D4, customCarData[PoldekDriver::nSelectedVehicle].gearShift[1]);
	Patch<float>(0x4742D0, customCarData[PoldekDriver::nSelectedVehicle].gearShift[2]);
	Patch<float>(0x4742CC, customCarData[PoldekDriver::nSelectedVehicle].gearShift[3]);
	Patch<float>(0x4742C8, customCarData[PoldekDriver::nSelectedVehicle].gearShift[4]);


	Draw2DTextColor(0.01f, 0.95f, 22.0, 0.0, "PolMod by ermaccer - polish-abandonware.pl", 0xFF20E1E8);
	if (SettingsMgr->bEnableMP)
	MP::Events::OnMainMenuProcess();
	_asm jmp jmpContinue
}

void __declspec(naked) PolMod::HookCarTextName()
{
	static int jmpPoint = 0x4077E9;

	ProcessCustomInput();

	if (bEnableSelMenu)
	{
		Draw2DText(0.029f, 0.05f, 35.0, 0.0, "Z - zwiêksz wybór");
		Draw2DText(0.029f, 0.10f, 35.0, 0.0, "X - zmniejsz wybór");
		Draw2DText(0.029f, 0.15f, 35.0, 0.0, "C - zatwierdŸ wybór");
		sprintf(strTemp, "Wybór: %d", nTempSelection + 1);
		Draw2DText(0.029f, 0.20f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "Dostêpne auta: %d", carData.size());
		Draw2DText(0.029f, 0.25f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "Aktualne auto: %d", PoldekDriver::nSelectedVehicle + 1);
		Draw2DText(0.029f, 0.30f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "(%s) (%s)", carNames[nTempSelection].c_str(), carData[nTempSelection].carFile);
		Draw2DText(0.179f, 0.20f, 25.0, 0.0, strTemp);
	}

	sprintf(strTemp, "%s", carNames[PoldekDriver::nSelectedVehicle].c_str());
	pStringPointer = (int)&strTemp;
	_asm
	{
		push pStringPointer
		jmp jmpPoint
	}
}

void __declspec(naked) PolMod::HookTrackTextName()
{
	static int jmpPoint = 0x407632;


	sprintf(strTemp, "%s", trackNames[PoldekDriver::nSelectedTrack].c_str());
	pStringPointer = (int)&strTemp;
	_asm
	{
		push pStringPointer
		jmp jmpPoint
	}
}



void PolMod::HookHUDData()
{
	_asm
	{
		lea eax, ds : 0x4A8F88
		mov ecx, eax
		mov tmp_ecx, ecx
	}

	ProcessCustomInputInGame();

	if (bDebugMenu)
	{

		car_file car = *(car_file*)(0x4AA3E8 + 0xB38);

	   	sprintf(strTemp, "X: %.2f Y: %.2f Z: %f Rot: %.2f", *(float*)(0x4AA3E8 + 0xA3C), *(float*)(0x4AA3E8 + 0xA3C + 4), *(float*)(0x4AA3E8 + 0x9C0), *(float*)(0x4AA3E8 + 0x930));
	     Draw2DText(0.029f, 0.10f, 24.0, 0.0, strTemp);


		// ad
		if (GetAsyncKeyState(68))
			*(float*)(0x4AA3E8 + 0xA3C) += 0.1f;

		if (GetAsyncKeyState(65))
			*(float*)(0x4AA3E8 + 0xA3C) -= 0.1f;

		//ws
		if (GetAsyncKeyState(87))
			*(float*)(0x4AA3E8 + 0xA3C + 4) += 0.1f;

		if (GetAsyncKeyState(83))
			*(float*)(0x4AA3E8 + 0xA3C + 4) -= 0.1f;

		// qe

		if (GetAsyncKeyState(69))
			*(float*)(0x4AA3E8 + 0x930) += 0.005f;

		if (GetAsyncKeyState(81))
			*(float*)(0x4AA3E8 + 0x930) -= 0.005f;

		// x
		if (GetAsyncKeyState(88))
			*(float*)(0x4AA3E8 + 0x9C0) += 0.05f;



	}

   if (SettingsMgr->bEnableMP)
	MP::Events::OnGameProcess();
}

void __fastcall PolMod::HookTrackPositionRace(int ptr, float x, float y, float z)
{

	float newx = trackData[PoldekDriver::nSelectedTrack].x;
	float newy = trackData[PoldekDriver::nSelectedTrack].y;


	SetPosition(ptr, newx, newy, trackData[PoldekDriver::nSelectedTrack].r);
}

void __fastcall PolMod::HookTrackPositionAIRace(int ptr, float x, float y, float z)
{
	float fAdjustX = 0.0f, fAdjustY = 0.0f;
	switch (ptr)
	{
	case AI_POINTER:
		fAdjustX = -customTrackData[PoldekDriver::nSelectedTrack].fAdjustX;
		fAdjustY = -customTrackData[PoldekDriver::nSelectedTrack].fAdjustY;
		break;
	case AI2_POINTER:
		fAdjustX = -customTrackData[PoldekDriver::nSelectedTrack].fAdjustX;
		fAdjustY = 0.0f;
		break;
	case AI3_POINTER:
		fAdjustX = 0.0f;
		fAdjustY = -customTrackData[PoldekDriver::nSelectedTrack].fAdjustY;
		break;
	}

	AI::SetPosition(ptr, trackData[PoldekDriver::nSelectedTrack].x + fAdjustX, trackData[PoldekDriver::nSelectedTrack].y + fAdjustY, trackData[PoldekDriver::nSelectedTrack].r);
}

int __fastcall PolMod::Hook41D2D0(int ptr, float rot, int unk)
{
	return 	((int(__thiscall*)(int, float, int))0x41D2D0)(ptr, rot,unk);
}

void  __fastcall PolMod::HookDisplayTrackImage(int ptr, int id, float x, float y, float sizex, float sizey, int unk)
{
	printf("PTR %x Image %d Pos %f %f Size %f %f Unk %d\n", ptr, id, x, y, sizex, sizey, unk);
	MessageBoxA(0, 0, 0, 0);
	//((void(__thiscall*)(int,int, float, float, float, float, int))0x40C810)(ptr,id,x,y,sizex,sizey,unk);
}


void __declspec(naked) PolMod::HookLoadTrack()
{
	static int loadTrack = 0x408385;

	pTrackPointer = MP::Events::GetServerMapName();
	printf("%s\n", (char*)pTrackPointer);
	_asm
	{
		lea eax, ds:0x4AE5A0
		mov ecx, eax
		push pTrackPointer
		jmp loadTrack
	}
}


void PolMod::GenPathPointer()
{
	pathData = std::make_unique<path[]>(trackPaths.size());

	for (int i = 0; i < trackPaths.size(); i++)
	{
		sprintf(pathData[i].name, "%s", trackPaths[i].c_str());
	}

	pPathPointer = (int)&pathData[0];
}

void __declspec(naked)PolMod::UpdatePathPointer()
{
}



void __declspec(naked) PolMod::HookLoadPath()
{
	static int loadPath = 0x407FE9;
	pPathPointer = (int)&pathData[PoldekDriver::nSelectedTrack];
	if (SettingsMgr->bEnableMP)
	pPathPointer = MP::Events::GetServerPathName();
	printf("Wczytywanie pth %s\n", (char*)pPathPointer);

	_asm
	{ 
		lea eax, ds:0x4AE120
		mov ecx, eax
		push pPathPointer
		jmp loadPath
	}
}

void PolMod::SetupCarRandomizer()
{
	srand(time(NULL));
}

int PolMod::GetRandomizedCarID()
{
	int id = rand() % ai_carData.size();
	//printf("wylosowalo %d (%s)\n", id,ai_carData[id].carFile);
	return id;
}

void __declspec(naked)PolMod::HookCreateAICarOne()
{
	static int loadArc1 = 0x408065;
	nLastRandomNumber = GetRandomizedCarID();
	pCarMarPointer1 = (int)&ai_carData[nLastRandomNumber].marArchive;
	if (SettingsMgr->bEnableMP)
		pCarMarPointer1 = MP::Events::GetServerCarAIArchiveName();
	printf("auto 1 %s ", (char*)pCarMarPointer1);

	__asm {
		push pCarMarPointer1
		jmp loadArc1

	}
}

void __declspec(naked) PolMod::HookCreateAICarTwo()
{
	static int car1 = 0x408077;
	pCarPointer1 = (int)&ai_carData[nLastRandomNumber].carFile;
	if (SettingsMgr->bEnableMP)
		pCarPointer1 = MP::Events::GetServerCarAIName();

	printf("%s\n", (char*)pCarPointer1);
	_asm {
		push pCarPointer1
		jmp car1

	}

}



void __declspec(naked)PolMod::HookCreateAI2CarOne()
{
	static int loadArc2 = 0x408136;
	nLastRandomNumber = GetRandomizedCarID();
	pCarMarPointer2 = (int)&ai_carData[nLastRandomNumber].marArchive;
	if (SettingsMgr->bEnableMP)
		pCarMarPointer2 = MP::Events::GetServerCarAIArchive2Name();
	printf("auto 2 %s ", (char*)pCarMarPointer2);

	__asm {
		push pCarMarPointer2
		jmp loadArc2

	}
}

void __declspec(naked) PolMod::HookCreateAI2CarTwo()
{
	static int car2 = 0x408148;
	pCarPointer2 = (int)&ai_carData[nLastRandomNumber].carFile;
	if (SettingsMgr->bEnableMP)
		pCarPointer2 = MP::Events::GetServerCarAI2Name();

	printf("%s\n", (char*)pCarPointer2);
	_asm {
		push pCarPointer2
		jmp car2

	}

}



void __declspec(naked)PolMod::HookCreateAI3CarOne()
{
	static int loadArc3 = 0x408208;
	nLastRandomNumber = GetRandomizedCarID();
	pCarMarPointer3 = (int)&ai_carData[nLastRandomNumber].marArchive;
	if (SettingsMgr->bEnableMP)
		pCarMarPointer3 = MP::Events::GetServerCarAIArchive3Name();
	printf("auto 3 %s ", (char*)pCarMarPointer2);

	__asm {
		push pCarMarPointer3
		jmp loadArc3

	}
}

void __declspec(naked) PolMod::HookCreateAI3CarTwo()
{
	static int car3 = 0x40821A;
	pCarPointer3 = (int)&ai_carData[nLastRandomNumber].carFile;
	if (SettingsMgr->bEnableMP)
		pCarPointer3 = MP::Events::GetServerCarAI3Name();

	printf("%s\n", (char*)pCarPointer3);
	_asm {
		push pCarPointer3
		jmp car3

	}

}
