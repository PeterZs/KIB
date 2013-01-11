#include "HandsKidget.h"

enum {
VIS_NEON,
VIS_NONE,
};
HandsKidget::HandsKidget(void)
{
}
HandsKidget::HandsKidget(Json::Value* v)
{
	oscname = (*v)["hands_osc_msg"].asString();
	if ((*v)["hands_visualization"].asString() == "hands_vis_neon") {
		vis = VIS_NEON;
	} else {
		vis = VIS_NONE;
	}
	parseColor((*v)["hands_color"].asString(), color);
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
			(float) l.x/l.w << (float) l.y/l.w << (float) l.z/l.w <<(float)  r.x/r.w << (float) r.y/r.w <<(float)  r.z/r.w <<
			osc::EndMessage;
	}
}

void HandsKidget::draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){
	long px[NUI_SKELETON_POSITION_COUNT];
	long py[NUI_SKELETON_POSITION_COUNT];
	USHORT tmpz;
	long tmpx, tmpy;
	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) {
		NuiTransformSkeletonToDepthImage(skeletonPositions[i], &tmpx, &tmpy, &tmpz, NUI_IMAGE_RESOLUTION_640x480);
		NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
																	tmpx, tmpy, tmpz, &px[i], &py[i]); 
	}
	int nl = NUI_SKELETON_POSITION_HAND_LEFT;
	int nr = NUI_SKELETON_POSITION_HAND_RIGHT;
	float radius = 20;
	glColor4f(color[0]/255.f, color[1]/255.f, color[2]/255.f, 0.5);
	fillArc(px[nl], py[nl], radius, 0, 2*M_PI, 50, -0.1);
	fillArc(px[nr], py[nr], radius, 0, 2*M_PI, 50, -0.1);
}