#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "osc/OscOutboundPacketStream.h"
#include "json\json.h"

class Kidget {
public:
	Kidget(void) {;}
	~Kidget(void) {;}

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions) = 0;
	virtual void draw(const Vector4* skeletonPositions, char* rgb, char* depth) = 0;
};

