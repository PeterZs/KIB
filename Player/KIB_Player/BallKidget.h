#pragma once
#include "kidget.h"
class BallKidget :
	public Kidget
{
public:
	BallKidget(void);
	~BallKidget(void);
	BallKidget(Json::Value v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, char* rgb, char* depth);
};

