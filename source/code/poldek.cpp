#include "..\pch.h"
#include "poldek.h"

int& PoldekDriver::nSelectedTrack = *(int*)0x0;
int& PoldekDriver::nSelectedVehicle = *(int*)0x494DE0;
int& PoldekDriver::nCurrentCamera = *(int*)0x4AAD08;

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
