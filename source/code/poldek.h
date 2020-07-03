#pragma once


struct car_info {
	char    carFile[64] = {};
	char    marArchive[64] = {};
	float   unknownFloat;
	int     unlocked;
};

struct track_info {
	char trackFile[64];
	char marArchive[64];
	int  unlocked;
	float x, y, z, rot;
};

struct track_details {
	char pathFile[64];
	char previewImage[64];
	char marArchive[140];
	float x, y, z, rot;
};


struct custom_car_info {
	bool   noCollisionData;
	bool   mrGear;
	float  gearShift[5];
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


class PoldekDriver {
public:
	static int& nSelectedVehicle;
	static int& nSelectedTrack;
	static int& nCurrentCamera;

};

namespace PoldekDriverFunctions {
	void Draw2DText(float x, float y, float scale, float unk, char* text);
	void Draw2DTextColor(float x, float y, float scale, float unk, char* text, int rgba);
}

