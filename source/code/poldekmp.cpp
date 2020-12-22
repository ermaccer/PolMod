#include "poldekmp.h"
#include "poldek.h"
#include "polmod.h"
#include "eSettingsManager.h"
#include "..\MemoryMgr.h"

using namespace Memory::VP;
using namespace PoldekDriverFunctions;

char mapBuffer[256];
char pathBuffer[256];

char car1Buffer[128];
char car2Buffer[128];
char car3Buffer[128];

char carArchive1Buffer[128];
char carArchive2Buffer[128];
char carArchive3Buffer[128];

char textBuffer[512];


void MP::InitHooks()
{
	Nop(0x41759D, 6);
	Nop(0x417608, 6);
	Nop(0x417745, 6);
}



void MP::SetPlayerPosition(ePlayerIDs ID, float X, float Y, float Rotation)
{
	X *= 10;
	Y *= 10;
	switch (ID)
	{
	case PLAYER:
		SetPosition(PLAYER_POINTER, X, Y, Rotation); break;
	case AI1:
		AI::SetPosition(AI_POINTER, X, Y, Rotation); break;
	case AI2:
		AI::SetPosition(AI2_POINTER, X, Y, Rotation); break;
	case AI3:
		AI::SetPosition(AI3_POINTER, X, Y, Rotation); break;
	}
}

void MP::RefreshCar(ePlayerIDs ID, char * car, char * mar)
{
	switch (ID)
	{
	case PLAYER:
		break;
	case AI1:
		SetCurrentArchive(mar);
		AI::LoadCarData(AI_POINTER, 0, car);
		CloseArchive();
		break;
	case AI2:
		SetCurrentArchive(mar);
		AI::LoadCarData(AI2_POINTER, 1, car);
		CloseArchive();
		break;
	case AI3:
		SetCurrentArchive(mar);
		AI::LoadCarData(AI3_POINTER, 2, car);
		CloseArchive();
		break;
	}
}


void MP::Events::OnGameLoad()
{


}

void MP::Events::OnGameProcess()
{

	
}


void MP::Events::OnMainMenuProcess()
{
	DrawConnectionStatus();
	
}




void MP::Events::DrawConnectionStatus()
{
	/*
	if (bConnected)
	{
		sprintf(textBuffer, "Po³¹czono z serwerem %s", SettingsMgr->szIP);
		Draw2DTextColor(0.01f, 0.75f, 22.0, 0.0, textBuffer, 0xFF30FF30);
	}
	else
	{
		sprintf(textBuffer, "B³¹d po³¹czenia z serwerem %s", SettingsMgr->szIP);
		Draw2DTextColor(0.01f, 0.75f, 22.0, 0.0, textBuffer, 0xFFFF3030);
	}
	*/
}

int MP::Events::GetServerMapName()
{
	sprintf(mapBuffer, "%s", "data\\wolka.mar");
	return (int)&mapBuffer;
}

int MP::Events::GetServerPathName()
{
	sprintf(pathBuffer, "%s", "wolka1.pth");
	return (int)&pathBuffer;
}

int MP::Events::GetServerCarAIName()
{
	sprintf(car1Buffer, "%s", "blackbeast.car");
	return (int)&car1Buffer;
}

int MP::Events::GetServerCarAI2Name()
{
	sprintf(car2Buffer, "%s", "black.car");
	return (int)&car2Buffer;
}

int MP::Events::GetServerCarAI3Name()
{
	sprintf(car3Buffer, "%s", "blackbeast.car");
	return (int)&car3Buffer;
}

int MP::Events::GetServerCarAIArchiveName()
{
	sprintf(carArchive1Buffer, "%s", "data\\blackbeast.mar");
	return (int)&carArchive1Buffer;
}

int MP::Events::GetServerCarAIArchive2Name()
{
	sprintf(carArchive2Buffer, "%s", "data\\black_dres.mar");
	return (int)&carArchive2Buffer;
}

int MP::Events::GetServerCarAIArchive3Name()
{
	sprintf(carArchive3Buffer, "%s", "data\\black_dres.mar");
	return (int)&carArchive3Buffer;
}
