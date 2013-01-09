#include "BallKidget.h"
enum {
	VIS_NONE,
	VIS_LIGHTNING,
	VIS_LINE,
	VIS_BALL,
};

BallKidget::BallKidget(void)
{
}
BallKidget::BallKidget(Json::Value* v)
{
	oscname = (*v)["ball_osc_msg"].asString();

	if ((*v)["ball_visualization"].asString() == "ball_vis_line") {
		vis = VIS_LINE;
	} else if ((*v)["ball_visualization"].asString() == "ball_vis_lightning") {
		vis = VIS_LIGHTNING;
	} else if ((*v)["ball_visualization"].asString() == "ball_vis_ball") {
		vis = VIS_BALL;
	} else {
		vis = VIS_NONE;
	}
	parseColor((*v)["ball_color"].asString(), color);
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

void BallKidget::draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid) {
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
	if (vis == VIS_LINE) {
		glColor3f(color[0]/255.f, color[1]/255.f, color[2]/255.f);
		glLineWidth(2.f);
		glBegin(GL_LINES);
			drawLine(px,py,nl, nr, -0.1);
		glEnd();
	} else if (vis == VIS_BALL) {
		glColor3f(color[0]/255.f, color[1]/255.f, color[2]/255.f);
		glLineWidth(2.f);
		glBegin(GL_LINES);
			drawLine(px,py,nl,nr, -0.1);
		glEnd();
		float midx = (px[nr] + px[nl])/2.f;
		float midy = (py[nr] + py[nl])/2.f;
		float r1 = dist(midx, midy, px[nr], py[nr]);
		float r2 = 5;
		DrawArc(midx, midy, r1, 0, 2*M_PI, 50, -0.1);
		DrawArc(midx, midy, r2, 0, 2*M_PI, 50, -0.1);
	}
}