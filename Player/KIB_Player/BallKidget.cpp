#include "BallKidget.h"


BallKidget::BallKidget(void)
{
}
BallKidget::BallKidget(Json::Value* v)
{
	oscname = (*v)["ball_osc_msg"].asString();
}

BallKidget::~BallKidget(void)
{
}

void BallKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){
	const Vector4& l = skeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
	const Vector4& r = skeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
	if (isPositionValid(l) && isPositionValid(r)) {
		p << osc::BeginMessage(oscname.c_str()) << 
			dist(l,r) << atan2(l.y/l.w - r.y/r.w, l.x/l.w - r.x/r.w) <<
			osc::EndMessage;
	}
}