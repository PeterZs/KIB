#pragma once
#include "kidget.h"
class BackgroundKidget :
	public Kidget
{
public:
	BackgroundKidget(void);
	~BackgroundKidget(void);
	BackgroundKidget(Json::Value v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, char* rgb, char* depth);
};

