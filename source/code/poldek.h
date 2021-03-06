#pragma once

#define PLAYER_POINTER 0x4AA3E8
#define AI_POINTER     0x4ABA58
#define AI2_POINTER    0x4AC744
#define AI3_POINTER    0x4AD430


struct vector {
	float x, y, z;
};
struct car_info {
	char    carFile[64] = {};
	char    marArchive[64] = {};
	float   unknownFloat;
	int     unlocked;
};

struct track_info {
	char trackFile[64] = {};
	char marArchive[64] = {};
	int unlock;
	int unk;
	float x;
	float y;
	float r;
};

struct track_details {
	vector plr;
	vector ai1;
	vector ai2;
	vector ai3;
};


struct custom_car_info {
	bool   noCollisionData;
	bool   mrGear;
	float  gearShift[5];
	bool   isAI;
};

struct car_file {
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


enum eCameraTypes {
	CAMERA_REAR,
	CAMERA_INSIDE,
	CAMERA_CINEMATIC
};

// keys

enum eDirectInputKeys {
	KEY_
};


class PoldekDriver {
public:
	static int& nSelectedVehicle;
	static int& nSelectedTrack;
	static int& nCurrentCamera;

	static int& nUnlockedTracks;
	static int& nUnlockedCars;
};

namespace PoldekDriverFunctions {
	void Draw2DText(float x, float y, float scale, float unk, char* text);
	void Draw2DTextColor(float x, float y, float scale, float unk, char* text, int rgba);
	void DrawImageRuntime(int id, float x, float y, float sizex, float sizey, int unk);
	void BeginDraw();
	void EndDraw();
	void SetCurrentArchive(const char* file);
	void CloseArchive();
	void LoadCarData(int vehicle, const char* file);
	void ResetVehicle(int vehicle);
	void __fastcall SetPosition(int vehicle, float x, float y, float rot);
	void Setup3DScene(float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10);
	namespace AI {
		void LoadCarData(int vehicle, int slot, const char* file);
		void ResetVehicle(int vehicle);
		void __fastcall SetPosition(int vehicle, float x, float y, float rot);
		void Init(int slot);
	}
}


// input (todo - array)

