#include "HandsKidget.h"


HandsKidget::HandsKidget(void)
{
}
HandsKidget::HandsKidget(Json::Value* v)
{
	oscname = (*v)["hands_osc_msg"].asString();
}

HandsKidget::~HandsKidget(void)
{
}

void HandsKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions) {
	const Vector4& l = skeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
	const Vector4& r = skeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
	if (isPositionValid(l) || isPositionValid(r)) {
		//printf("OSC Message %.3f %.3f %.3f\n", l.x, l.y, l.z);
		p << osc::BeginMessage(oscname.c_str()) << 
			l.x/l.w << l.y/l.w << l.z/l.w << r.x/r.w << r.y/r.w << r.z/r.w <<
			osc::EndMessage;
	}
}