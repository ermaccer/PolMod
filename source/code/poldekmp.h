#pragma once
#include "poldek.h"


enum ePlayerIDs {
	PLAYER,
	AI1,
	AI2,
	AI3,
	TOTAL_PLAYERS
};

namespace MP {
	namespace Events {
		void OnGameLoad();
		void OnGameProcess();
		void OnMainMenuProcess();
		void DrawConnectionStatus();

		int  GetServerMapName();
		int  GetServerPathName();
		int  GetServerCarAIName();
		int  GetServerCarAI2Name();
		int  GetServerCarAI3Name();

		int  GetServerCarAIArchiveName();
		int  GetServerCarAIArchive2Name();
		int  GetServerCarAIArchive3Name();
	}

	void InitHooks();
	void SetPlayerPosition(ePlayerIDs ID, float X, float Y, float Rotation);
	void RefreshCar(ePlayerIDs ID, char* car, char* mar);
}
