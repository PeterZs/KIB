#include "WaveKidget.h"


WaveKidget::WaveKidget(void)
{
}
WaveKidget::WaveKidget(Json::Value* v)
{
	oscname = (*v)["wave_osc_msg"].asString();
}

WaveKidget::~WaveKidget(void)
{
}

inline double angle(const Vector4& a, const Vector4& b) {
	return atan2(a.y/a.w - b.y/b.w, a.x/a.w - b.x/b.w);
}

void WaveKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){
	const Vector4& l = skeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
	const Vector4& le = skeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT];
	const Vector4& r = skeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
	const Vector4& re = skeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT];
	if (isPositionValid(l) || isPositionValid(r)) {
		p << osc::BeginMessage(oscname.c_str());
		if (isPositionValid(l)) {
			p << angle(l,le);
		} else {
			p << 0.;
		}
		if (isPositionValid(r)) {
			p << angle(r,re);
		} else {
			p << 0.;
		}
		p << osc::EndMessage;
	}
}