#include "..\pch.h"
#include "poldek.h"
#include "polmod.h"
#include <vector>
#include <string>
#include <filesystem>
#include "..\IniReader.h"
#include "..\MemoryMgr.h"
#include <time.h>

using namespace Memory::VP;
using namespace PoldekDriverFunctions;

int tmp_ecx = 0;

char strTemp[256];
int  pStringPointer = 0;



bool bEnableSelMenu = 0;
bool bDebugSwitch = 0;
int nTempSelection = 0;
static int keyTimer = GetTickCount();


std::vector<car_info> carData;
std::vector<custom_car_info> customCarData;

std::vector<track_info> trackData;
std::vector<track_details> trackInfo;
std::vector<int> trackSecondVariant;

std::vector<std::string> carNames;
std::vector<std::string> trackNames;
std::vector<std::string> carFilePaths;
std::vector<std::string> trackFilePaths;


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

void PolMod::LoadCarData(char * file)
{
	CIniReader ini(file);
	car_info car;
	custom_car_info cc;
	sprintf(car.marArchive, ini.ReadString("Car", "MarArchive", 0));
	sprintf(car.carFile, ini.ReadString("Car", "CarFile", 0));
	car.unlocked = ini.ReadInteger("Car", "Unlocked", 1);	
	car.unknownFloat = ini.ReadFloat("Car", "UnknownFloat", 0.0f);
	cc.noCollisionData = ini.ReadBoolean("Car", "NoDamagedParts", false);
	cc.mrGear = ini.ReadBoolean("Car", "Use4Gears", false);

	cc.gearShift[0] = ini.ReadFloat("GearShift", "1", 1.0f);
	cc.gearShift[1] = ini.ReadFloat("GearShift", "2", 30.0f);
	cc.gearShift[2] = ini.ReadFloat("GearShift", "3", 50.0f);
	cc.gearShift[3] = ini.ReadFloat("GearShift", "4", 80.0f);
	cc.gearShift[4] = ini.ReadFloat("GearShift", "5", 120.0f);

	char temp[256];
	sprintf(temp, "%s", ini.ReadString("Car", "Name", "No name"));
	std::string str(temp, strlen(temp));

	carData.push_back(car);
	customCarData.push_back(cc);
	carNames.push_back(str);
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
	if (GetAsyncKeyState(VK_F1))
	{
		if (GetTickCount() - keyTimer <= 150) return;
		keyTimer = GetTickCount();
		bDebugSwitch ^= 1;
	}

}

void PolMod::DoPatches()
{
	// liczba
	int cars = carData.size();
	Patch<int>(0x41AD25 + 6, cars);

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


	// set default camera to rear view
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

void PolMod::HookHUDData()
{
	_asm
	{
		lea eax, ds : 0x4A8F88
		mov ecx, eax
		mov tmp_ecx, ecx
	}

	ProcessCustomInputInGame();

	if (bDebugSwitch)
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
}
