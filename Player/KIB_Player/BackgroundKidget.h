#pragma once
#include "kidget.h"
#include <gl\GL.h>

class BackgroundKidget :
	public Kidget
{
public:
	BackgroundKidget(void);
	~BackgroundKidget(void);
	BackgroundKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){;}
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid);

protected:
	unsigned char fg[3];
	unsigned char bg[2][3];
	long px[NUI_SKELETON_POSITION_COUNT];
	long py[NUI_SKELETON_POSITION_COUNT];
	int bgvis;
	int fgvis;
	GLuint textureId;
	std::vector<double> starsr;
	std::vector<double> starst;
	std::vector<double> starsv;

	unsigned long last_update;
	int width;
	int height;
	unsigned char* data;
	char* colorspaceplayer;
	float* grad;
};

