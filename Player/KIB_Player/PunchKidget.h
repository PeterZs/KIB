#pragma once
#include "kidget.h"
class PunchKidget :
	public Kidget
{
public:
	PunchKidget(void);
	~PunchKidget(void);
	PunchKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){;}

protected:
	int statel;
	int stater;

	double getArmlength(const Vector4* skeletonPositions) {
		return 0.5;
	}
};

