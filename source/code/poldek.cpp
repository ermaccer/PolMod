#include "..\pch.h"
#include "poldek.h"



int& PoldekDriver::nSelectedTrack = *(int*)0x494DE4;
int& PoldekDriver::nSelectedVehicle = *(int*)0x494DE0;
int& PoldekDriver::nCurrentCamera = *(int*)0x4AAD08;

int& PoldekDriver::nUnlockedCars= *(int*)0x494DEC;
int& PoldekDriver::nUnlockedTracks= *(int*)0x494DF0;

void PoldekDriverFunctions::Draw2DText(float x, float y, float scale, float unk, char * text)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4A96F8
		mov ecx, eax
		mov _ecx, ecx
	}
	((void(__thiscall*)(int, float, float, float, float, char*))0x40CDE0)(_ecx, x, y, scale, unk, text);

}	
void PoldekDriverFunctions::Draw2DTextColor(float x, float y, float scale, float unk, char * text, int rgba)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4A96F8
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int, float, float, float, float, char*, int))0x40CC60)(_ecx, x,y,scale,unk,text,rgba);
}

void PoldekDriverFunctions::DrawImageRuntime(int id, float x, float y, float sizex, float sizey, int unk)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4A96F8
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int, int, float, float, float, float, int))0x40C810)(_ecx, id, x, y, sizex, sizey, unk);
}

void PoldekDriverFunctions::BeginDraw()
{
	(*(void(__stdcall **)(int))(*(int*)*(int*)0x4A96FC + 164))(*(int*)0x4A96FC);
}

void PoldekDriverFunctions::EndDraw()
{
	(*(void(__stdcall **)(int))(*(int*)*(int*)0x4A96FC + 168))(*(int*)0x4A96FC);
}

void PoldekDriverFunctions::SetCurrentArchive(const char * file)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4F2AD8
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int, const char*))0x409AA0)(_ecx, file);
}

void PoldekDriverFunctions::CloseArchive()
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4F2AD8
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int))0x409B20)(_ecx);
}

void PoldekDriverFunctions::LoadCarData(int vehicle, const char * file)
{
	((void(__thiscall*)(int, const char*))0x405AC0)(vehicle, file);
}

void PoldekDriverFunctions::ResetVehicle(int vehicle)
{
	((void(__thiscall*)(int))0x404EC0)(vehicle);
}

void __fastcall PoldekDriverFunctions::SetPosition(int vehicle, float x, float y, float rot)
{
	((void(__thiscall*)(int,float,float,float))0x403F90)(vehicle,x,y,rot);
}

void PoldekDriverFunctions::Setup3DScene(float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x494F68
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int, float, float, float, float, float, float, float, float, float))0x403F90)(_ecx,a2,a3,a4,a5,a6,a7,a8,a9,a10);
}

void PoldekDriverFunctions::AI::LoadCarData(int vehicle, int slot, const char * file)
{
	((void(__thiscall*)(int,int, const char*))0x419280)(vehicle,slot, file);
}

void PoldekDriverFunctions::AI::ResetVehicle(int vehicle)
{
	((void(__thiscall*)(int))0x419040)(vehicle);
}

void __fastcall PoldekDriverFunctions::AI::SetPosition(int vehicle, float x, float y, float rot)
{
	((void(__thiscall*)(int, float, float, float))0x418110)(vehicle, x, y, rot);
}

void PoldekDriverFunctions::AI::Init(int slot)
{
	int _ecx = 0;
	_asm
	{
		lea eax, ds : 0x4AE120
		mov ecx, eax
		mov _ecx, ecx
	}

	((void(__thiscall*)(int, int))0x409CF0)(_ecx, slot);
}
