#pragma once
#include "kidget.h"
class WaveKidget :
	public Kidget
{
public:
	WaveKidget(void);
	~WaveKidget(void);
	WaveKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){;}
};

