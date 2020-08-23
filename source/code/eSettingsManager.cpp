#include "eSettingsManager.h"
#include <iostream>
eSettingsManager* SettingsMgr = new eSettingsManager();

void eSettingsManager::Init()
{
	ini = new CIniReader("");
	bWlaczKonsole = ini->ReadBoolean("Ustawienia", "bWlaczKonsole", false);
	bWylaczUszkodzeniaAI = ini->ReadBoolean("Ustawienia", "bWylaczUszkodzeniaAI", true);
	bWylaczIntro = ini->ReadBoolean("Ustawienia", "bWylaczIntro", true);
	bWylaczLogo = ini->ReadBoolean("Ustawienia", "bWylaczLogo",true);

	bEnableMP = ini->ReadBoolean("Multiplayer", "bWlacz", false);
	sprintf(szNick, "%s", ini->ReadString("Multiplayer", "szNick", "NowySebol"));
	sprintf(szIP, "%s", ini->ReadString("Multiplayer", "szIP", "127.0.0.1"));
}
