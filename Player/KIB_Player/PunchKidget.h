#pragma once
#include "kidget.h"
class PunchKidget :
	public Kidget
{
public:
	PunchKidget(void);
	~PunchKidget(void);
	PunchKidget(Json::Value v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, char* rgb, char* depth);
};

