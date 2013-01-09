#include "ClapKidget.h"
enum {
	DEFAULT,
	START,
	CLAPPING
};

const double ClapKidget::radius = 0.04;

ClapKidget::ClapKidget(void)
{
}
ClapKidget::ClapKidget(Json::Value* v)
{
	oscname = (*v)["clap_osc_msg"].asString();
	state = DEFAULT;
}

ClapKidget::~ClapKidget(void)
{
}

void ClapKidget::sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions){
	const Vector4& l = skeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
	const Vector4& r = skeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
	if (isPositionValid(l) && isPositionValid(r)) {
		if (dist(l,r) < radius) {
			if (state == DEFAULT) state = START;
			else if (state == START) {
				state = CLAPPING;
				p << osc::BeginMessage(oscname.c_str()) << osc::EndMessage;
			}
		} else {
			state = DEFAULT;
		}
	}
}