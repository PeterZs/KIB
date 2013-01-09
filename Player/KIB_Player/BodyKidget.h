#pragma once
#include "kidget.h"
class BodyKidget :
	public Kidget
{
public:
	BodyKidget(void);
	~BodyKidget(void);

	BodyKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){;}
};

