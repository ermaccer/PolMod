#include "poldekmp.h"
#include "poldek.h"
#include <Windows.h>
#include <memory>
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

void MP::InitHooks()
{
	Nop(0x41759D, 6);
	Nop(0x417608, 6);
	Nop(0x417745, 6);
}

void MP::SetPlayerPosition(ePlayerIDs ID, float X, float Y, float Rotation)
{
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

void MP::Events::OnGameLoad()
{
}

void MP::Events::OnGameProcess()
{
}

void MP::Events::OnMainMenuProcess()
{
}
int MP::Events::GetServerMapName()
{
	sprintf(mapBuffer, "%s", "TODO");
	return (int)&mapBuffer;
}

int MP::Events::GetServerPathName()
{
	sprintf(pathBuffer, "%s", "Path - TODO");
	return (int)&pathBuffer;
}

int MP::Events::GetServerCarAIName()
{
	sprintf(car1Buffer, "%s", "Car 1 - TODO");
	return (int)&car1Buffer;
}

int MP::Events::GetServerCarAI2Name()
{
	sprintf(car2Buffer, "%s", "Car 2 - TODO");
	return (int)&car2Buffer;
}

int MP::Events::GetServerCarAI3Name()
{
	sprintf(car3Buffer, "%s", "Car 3 - TODO");
	return (int)&car3Buffer;
}

int MP::Events::GetServerCarAIArchiveName()
{
	sprintf(carArchive1Buffer, "%s", "Car 1 - TODO");
	return (int)&car1Buffer;
}

int MP::Events::GetServerCarAIArchive2Name()
{
	return 0;
}

int MP::Events::GetServerCarAIArchive3Name()
{
	return 0;
}

