#pragma once
#include "kidget.h"
class ArcKidget :
	public Kidget
{
public:
	ArcKidget(void);
	~ArcKidget(void);

	ArcKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){;}
};

