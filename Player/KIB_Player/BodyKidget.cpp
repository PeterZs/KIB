#include "BodyKidget.h"


BodyKidget::BodyKidget(void)
{
}
BodyKidget::BodyKidget(Json::Value* v)
{
	oscname = (*v)["body_osc_msg"].asString();
}

BodyKidget::~BodyKidget(void)
{
}

void BodyKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){
	const Vector4& head = skeletonPositions[NUI_SKELETON_POSITION_HEAD];
	const Vector4& hip = skeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	if (isPositionValid(head) && isPositionValid(hip)) {
		p << osc::BeginMessage(oscname.c_str()) << 
			atan2(head.x/head.w - hip.x/hip.w, head.y/head.w - hip.y/hip.w) <<
			osc::EndMessage;
	}
}