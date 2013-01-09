#include "PunchKidget.h"
enum {
	DEFAULT,
	START,
	PUNCHING
};

PunchKidget::PunchKidget(void)
{
}

PunchKidget::PunchKidget(Json::Value* v)
{
	oscname = (*v)["punch_osc_msg"].asString();
	statel = DEFAULT;
	stater = DEFAULT;
}
PunchKidget::~PunchKidget(void)
{
}
void PunchKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){
	const Vector4& l = skeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
	const Vector4& r = skeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
	const Vector4& h = skeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	bool sent = false;
	if (isPositionValid(l)) {
		if (h.z-h.w - l.z/l.w > getArmlength(skeletonPositions)) {
			if (statel == DEFAULT) statel = START;
			else if (statel == START) {
				statel = PUNCHING;
				p << osc::BeginMessage(oscname.c_str()) << osc::EndMessage;
				sent = true;
			}
		} else {
			statel = DEFAULT;
		}
	}
	if (isPositionValid(r)) {
		if (h.z-h.w - r.z/r.w > getArmlength(skeletonPositions)) {
			if (stater == DEFAULT) stater = START;
			else if (stater == START) {
				stater = PUNCHING;
				if (!sent) {
					p << osc::BeginMessage(oscname.c_str()) << osc::EndMessage;
				}
			}
		} else {
			stater = DEFAULT;
		}
	}
}