#pragma once
#include "kidget.h"
class HandsKidget :
	public Kidget
{
public:
	HandsKidget(void);
	~HandsKidget(void);
	HandsKidget(Json::Value v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, char* rgb, char* depth);
};

