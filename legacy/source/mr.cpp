#include "pch.h"
#include "mr.h"
#include <vector>
#include <string>
#include <filesystem>
#include "IniReader.h"
#include "MemoryMgr.h"
#include <time.h>


// mr stuff
int& nSelectedCar = *(int*)0x494670;
int& nSelectedTrack = *(int*)0x494674;
int& pImagesPointer = *(int*)0x4A8F88;
int& pModelsPointer = *(int*)0x4947F8;
int& pDataPointer = *(int*)0x4F2360;

int& bRaceMode = *(int*)0x48479C;



char strTemp[256];
int  pStringPointer = 0;
int  nLastRandomizedCar = 0;

// hook text
int tmp_ecx;
int nTempSelection = 0;
int nCars = 0;
int nTracks = 0;
bool bEnableSelMenu;
bool bDebugSwitch = 0;

// menu timer
static int keyTimer = GetTickCount();

// custom flags
bool bResourcesLoaded = false;


using namespace Memory::VP;

std::vector<CCarInfo> carData;
std::vector<CCustomCarInfo> customCarData;

std::vector<CTrackInfo> trackData;
std::vector<CTrackDetails> trackInfo;
std::vector<int> trackSecondVariant;

std::vector<std::string> carNames;
std::vector<std::string> trackNames;
std::vector<std::string> carFilePaths;
std::vector<std::string> trackFilePaths;

void LoadCarFiles(const char* folder)
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

void LoadTrackFiles(const char * folder)
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
			LoadTrackData((char*)carFilePaths[i].c_str());
	}
}

void LoadCarData(char * file)
{
	CIniReader ini(file);
	CCarInfo car;
	CCustomCarInfo cc;
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

void LoadTrackData(char * file)
{
	CIniReader ini(file);
	CTrackInfo track;
	CTrackDetails det;

	sprintf(track.trackFile, ini.ReadString("Track", "TrackFile", 0));
	sprintf(track.marArchive, ini.ReadString("Track", "MarArchive", 0));
	track.x = ini.ReadFloat("Track", "X", 0);
	track.y = ini.ReadFloat("Track", "Y", 0);
	track.z = ini.ReadFloat("Track", "Z", 0);
	track.rot = ini.ReadFloat("Track", "X", 0);


	sprintf(det.pathFile, ini.ReadString("TrackInfo", "PathFile", 0));
	sprintf(det.marArchive, ini.ReadString("TrackInfo", "MarArchive", 0));
	sprintf(det.previewImage, ini.ReadString("TrackInfo", "Preview", 0));
	det.x = ini.ReadFloat("TrackInfo", "X", 0);
	det.y = ini.ReadFloat("TrackInfo", "Y", 0);
	det.z = ini.ReadFloat("TrackInfo", "Z", 0);
	det.rot = ini.ReadFloat("TrackInfo", "X", 0);

	int route = ini.ReadInteger("TrackInfo", "AlternateRoute", 0);
	trackSecondVariant.push_back(route);


	char temp[256];
	sprintf(temp, "%s", ini.ReadString("Track", "TrackFile", "No name"));
	std::string str(temp, strlen(temp));

	trackNames.push_back(str);
	trackData.push_back(track);
	trackInfo.push_back(det);

}

int GetCarInfo(char* file)
{
	int iFind = 0;

	for (int i = 0; i < customCarData.size(); i++)
	{
		if (strcmp(carData[i].marArchive, file) == 0)
		{
			iFind = i;
			break;
		}
	}
	return iFind;
}

void __declspec(naked) HookCarTextName()
{
	static int jmpPoint = 0x4077D4;

	
	_asm
	{
		lea eax, ds : 0x4A8F88
		mov ecx, eax
		mov tmp_ecx, ecx
	}

	ProcessCustomInput();

	if (bEnableSelMenu)
	{
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.05f, 35.0, 0.0, "Z - zwiêksz wybór");
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.10f, 35.0, 0.0, "X - zmniejsz wybór");
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.15f, 35.0, 0.0, "C - zatwierdŸ wybór");
		sprintf(strTemp, "Wybór: %d", nTempSelection + 1);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.20f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "Dostêpne auta: %d", carData.size());
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.25f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "Aktualne auto: %d", nSelectedCar + 1);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.30f, 35.0, 0.0, strTemp);
		sprintf(strTemp, "(%s) (%s)", carNames[nTempSelection].c_str(), carData[nTempSelection].carFile);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.179f, 0.20f, 25.0, 0.0, strTemp);
	}






	sprintf(strTemp, "%s",carNames[nSelectedCar].c_str());
	pStringPointer = (int)&strTemp;
	_asm
	{
		push pStringPointer
		jmp jmpPoint	
	}
	 
}

void __declspec(naked) HookTrackTextName()
{
	static int jmpPoint = 0x40761D;

	sprintf(strTemp, "%s", trackNames[nSelectedTrack].c_str());
	pStringPointer = (int)&strTemp;
	_asm
	{
		push pStringPointer
		jmp jmpPoint
	}

}


void RandomizeCar()
{
	srand(time(NULL));

	nLastRandomizedCar = rand() % carData.size();
}

void HookEnemyAICarOne(char* file, int ptr)
{
	((void(__cdecl*)(char*,int))0x409A70)(file, ptr);
}

void  HookEnemyAICarTwo(int id, char * file)
{


	//char* info = new char[260];
	//memset(info, 0, 260);

	//strcat(info, "black.car");


	((void(__cdecl*)(int, char*))0x41B0A0)(id, file);
}

void ProcessCustomInput()
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
			nSelectedCar = nTempSelection;

	}


}

void ProcessCustomInputInGame()
{
	if (GetAsyncKeyState(VK_F1))
	{
		if (GetTickCount() - keyTimer <= 150) return;
		keyTimer = GetTickCount();
		bDebugSwitch ^= 1;
	}

	if (GetAsyncKeyState(82))
	{
		if (GetTickCount() - keyTimer <= 150) return;
		keyTimer = GetTickCount();
		
	}
}

void __declspec(naked) HookInfo()
{

	static int jmpContinue = 0x4070A0;
	_asm {
		mov eax, 0x40C7F0
		call eax
	}

	_asm
	{
		lea eax, ds : 0x4A8F88
		mov ecx, eax
		mov tmp_ecx, ecx
	}


	// set default camera to rear view
	Patch<int>(0x4AA590, 0);

	if (customCarData[nSelectedCar].mrGear)
		Patch<int>(0x401C38 + 6, 4);
	else
		Patch<int>(0x401C38 + 6, 5);

	if (customCarData[nSelectedCar].noCollisionData)
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

	Patch<float>(0x4742D8, customCarData[nSelectedCar].gearShift[0]);
	Patch<float>(0x4742D4, customCarData[nSelectedCar].gearShift[1]);
	Patch<float>(0x4742D0, customCarData[nSelectedCar].gearShift[2]);
	Patch<float>(0x4742CC, customCarData[nSelectedCar].gearShift[3]);
	Patch<float>(0x4742C8, customCarData[nSelectedCar].gearShift[4]);


	((void(__thiscall*)(int, float, float, float, float, char*,int))0x40CC40)(tmp_ecx, 0.01f, 0.95f, 22.0, 0.0, "PolMod by ermaccer - polish-abandonware.pl",0xFF20E1E8);

	_asm jmp jmpContinue
}

void __declspec(naked) HookCarAmount()
{
	static int jmpPoint = 0x41ACFC;

	_asm {
		mov eax, nCars
		mov ds:0x4AAECC, eax
		jmp jmpPoint
	}

}

void HookCollisionTest()
{

}


char * GetRaceMode()
{
	if (bRaceMode)
		return "Wyœcig";
	else
		return "Trening";
}

void HookHUDData()
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

		ECarFile car = *(ECarFile*)(0x4A9C70 + 0xB38);

		sprintf(strTemp, "Tryb gry: %s", GetRaceMode());
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.05f, 22.0, 0.0, strTemp);

		sprintf(strTemp, "X: %.2f Y: %.2f Z: %f Rot: %.2f", *(float*)(0x4A9C70 + 0xA3C), *(float*)(0x4A9C70 + 0xA3C + 4), *(float*)(0x4A9C70 + 0x9C0), *(float*)(0x4A9C70 + 0x930));
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.10f, 22.0, 0.0, strTemp);


		sprintf(strTemp, "Model: %s Felga: %s", car.szBodyMesh, car.szWheelMesh);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.15f, 22.0, 0.0, strTemp);


		sprintf(strTemp, "4 biegi: %d Bez zniszczen: %d", customCarData[nSelectedCar].mrGear, customCarData[nSelectedCar].noCollisionData);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.20f, 22.0, 0.0, strTemp);


		sprintf(strTemp, "Auta: %d Trasy: %d", nCars, nTracks);
		((void(__thiscall*)(int, float, float, float, float, char*))0x40CDC0)(tmp_ecx, 0.029f, 0.25f, 22.0, 0.0, strTemp);


		// ad
		if (GetAsyncKeyState(68))
			*(float*)(0x4A9C70 + 0xA3C) += 0.1f;

		if (GetAsyncKeyState(65))
			*(float*)(0x4A9C70 + 0xA3C) -= 0.1f;

		//ws
		if (GetAsyncKeyState(87))
			*(float*)(0x4A9C70 + 0xA3C + 4) += 0.1f;

		if (GetAsyncKeyState(83))
			*(float*)(0x4A9C70 + 0xA3C + 4) -= 0.1f;

		// qe

		if (GetAsyncKeyState(69))
			*(float*)(0x4A9C70 + 0x930) += 0.005f;

		if (GetAsyncKeyState(81))
			*(float*)(0x4A9C70 + 0x930) -= 0.005f;

		// x
		if (GetAsyncKeyState(88))
			*(float*)(0x4A9C70 + 0x9C0) += 0.05f;



	}

}





void DoPatches()
{
	// max vehicles
	nCars = carData.size();
	//nTracks = trackData.size();

	Patch<int>(0x407F2B + 3, (int)&carData[0]);
	Patch<int>(0x4082A7 + 3, (int)&carData[0]);

	Patch<int>(0x407F0D + 3, (int)&carData[0] + 64);
	Patch<int>(0x408289 + 3, (int)&carData[0] + 64);


	// unlocked status
	Patch<int>(0x41BF12 + 1, (int)&carData[0].unlocked);
	Patch<int>(0x406A20 + 1, (int)&carData[0].unlocked);
	Patch<int>(0x41A53A + 1, (int)&carData[0].unlocked);
	Patch<int>(0x406F76 + 3, (int)&carData[0].unlocked);
	Patch<int>(0x407005 + 3, (int)&carData[0].unlocked);
	Patch<int>(0x4077FB + 3, (int)&carData[0].unlocked);
	Patch<int>(0x4077FB + 3, (int)&carData[0].unlocked);



	Patch<int>(0x407FE8 + 3, (int)&carData[0].unknownFloat);


	/*
	// tracks
	Patch<int>(0x41ACAB + 1, trackData.size());
	Patch<int>(0x40835D + 3, (int)&trackData[0]);
	Patch<int>(0x408340 + 3, (int)&trackData[0] + 64);


	// variant
	Patch<int>(0x40754B + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x406E00 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x406E0C + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x406ED7 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x406EF2 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x40754B + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x407F56 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x407FAA + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x408007 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x40807C + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x4080DA + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x40814D + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x4081AB + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x40821F + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x4088AB + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x40891E + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x408968 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x4089C1 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x408A09 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x408A63 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x408AAD + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A2D4 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A317 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A362 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A394 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A3D4 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A417 + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A45B + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A4AB + 3, (int)&trackSecondVariant[0]);
	Patch<int>(0x41A4EA + 3, (int)&trackSecondVariant[0]);
	*/

	// decrease car font size
	Patch<float>(0x4077D6 + 1, 40.0f);




	//Patch<int>(0x4175F6 + 1, (int)&carData[0].unlocked);

	// was only used in mr
	//Patch<int>(0x407E1D + 1, (int)&carData[0].marArchive);


}

void __declspec(naked) HookSkipCollision()
{
	static int jmpPoint = 0x405CBB;
	_asm jmp jmpPoint
}


void Draw2DText(float x, float y, float scale, float unk, char * text)
{
	((void(__cdecl*)(float, float, float, float, char*))0x40CDC0)(x,y,scale,unk,text);
}

void Load2DImage(int ptr, int slot, const char * name)
{
	((void(__thiscall*)(int,int,const char*))0x40CF40)(ptr, slot,name);
}

void SetCurrentArchive(int ptr, const char * name)
{

	((void(__thiscall*)(int, const char*))0x409A80)(ptr, name);
}
