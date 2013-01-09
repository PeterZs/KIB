#pragma once
#include "kidget.h"
class ClapKidget :
	public Kidget
{
public:
	ClapKidget(void);
	~ClapKidget(void);
	ClapKidget(Json::Value* v);

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions);
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){;}
	
protected:
	int state;
	static const double radius;
};

