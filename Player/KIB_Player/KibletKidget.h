#pragma once
#include "kidget.h"
class KibletKidget :
	public Kidget
{
public:
	KibletKidget(void);
	~KibletKidget(void);
	KibletKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){;}
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid) { ; }
};

