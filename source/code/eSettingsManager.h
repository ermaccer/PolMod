#pragma once
#include "..\IniReader.h"

class eSettingsManager {
public:
	void Init();
	CIniReader* ini;

	bool bWlaczKonsole;
	bool bWylaczUszkodzeniaAI;
	bool bWylaczIntro;
	bool bWylaczLogo;

	bool bEnableMP;
	char szNick[128];
	char szIP[128];

};

extern eSettingsManager* SettingsMgr;