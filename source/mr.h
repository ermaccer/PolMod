#pragma once
struct CCarInfo{
	char carFile[64] = {};
	char marArchive[64] = {};
	float unknownFloat;
	int  unlocked;
};

struct CTrackInfo {
	char trackFile[64];
	char marArchive[64];
	int  unlocked;
	float x,y,z,rot;
};

struct CTrackDetails {
	char pathFile[64];
	char previewImage[64];
	char marArchive[140];
	float x, y, z, rot;
};


struct CCustomCarInfo {
	bool   noCollisionData;
	bool   mrGear;
	float  gearShift[5];
};

// todo
enum eNewGearImages {
	GEAR_MR_1 = 133,
	GEAR_MR_2,
	GEAR_MR_3,
	GEAR_MR_4,
	GEAR_MR_R,
	GEAR_MR_N
};


struct ECarFile {
	char header[8];
	float fPowerFactor;
	float fFrontTireMAX;
	float fRearTireMAX;
	float fBreakingForce;
	float fYCarPosition;
	float fSuspensionKU;
	float fSuspensionKV;
	char  szBodyMesh[64];
	char  szWheelMesh[64];
	float fWheelRadius;
	float fFrontWheelPos;
	float fRearWheelPos;
	float fBodyWidth;
	float fFrontBumper;
	float fRearBumper;
	int   bFrontWheelDrive;
	float fUnk[2];
	float fStopLight[3];
	float fFrontDirLight[3];
	float fRearDirLight[3];
	float fSideDirLight[3];
	float fReverseDirLight[3];
};

void LoadCarFiles(const char* folder);
void LoadTrackFiles(const char* folder);
void LoadCarData(char* file);
void LoadTrackData(char* file);
int  GetCarInfo(char* file);
void DoPatches();

void HookSkipCollision();


void HookCarTextName();
void HookTrackTextName();
void RandomizeCar();
void HookEnemyAICarOne(char* file, int ptr);
void HookEnemyAICarTwo(int id, char* file);
void ProcessCustomInput();
void ProcessCustomInputInGame();
void HookInfo();
void HookCarAmount();


char* GetRaceMode();

void HookHUDData();



// mr functions
void Draw2DText(float x, float y, float scale, float unk, char* text);
void Load2DImage(int ptr, int slot, const char* name);
void SetCurrentArchive(int ptr, const char* name);