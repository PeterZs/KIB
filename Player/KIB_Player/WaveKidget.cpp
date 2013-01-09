#include "WaveKidget.h"

enum{
	VIS_NONE,
	VIS_KNOB,
	VIS_PIE,
};

WaveKidget::WaveKidget(void)
{
}
WaveKidget::WaveKidget(Json::Value* v)
{
	oscname = (*v)["wave_osc_msg"].asString();

	if ((*v)["wave_visualization"].asString() == "wave_vis_knob") {
		vis = VIS_KNOB;
	} else if ((*v)["wave_visualization"].asString() == "wave_vis_pie") {
		vis = VIS_PIE;
	} else {
		vis = VIS_NONE;
	}
	lefthand = true; righthand = true;
	if ((*v)["wave_hands"].asString() == "wave_hands_left") {
		righthand = false;
	} else if ((*v)["wave_hands"].asString() == "wave_hands_right") {
		lefthand = false;
	}
	parseColor((*v)["wave_color"].asString(), color);
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

void WaveKidget::draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){
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
	int el = NUI_SKELETON_POSITION_ELBOW_LEFT;
	int er = NUI_SKELETON_POSITION_ELBOW_RIGHT;

	glColor3f(color[0]/255.f, color[1]/255.f, color[2]/255.f);
	glLineWidth(2.f);
	if (lefthand) {
		drawSingle(px[nl], py[nl], px[el], py[el]);
	}
	if (righthand) {
		drawSingle(px[nr], py[nr], px[er], py[er]);
	}
}

static float knobradius = 80;
void WaveKidget::drawSingle(int hx, int hy, int ex, int ey) {
	if (vis == VIS_KNOB) {
		double dp = dist(hx,hy,ex,ey);
		double px = (hx - ex)*knobradius/dp + ex;
		double py = (hy - ey)*knobradius/dp + ey;
		glBegin(GL_LINES);
			glVertex3f((float)ex, (float)ey, -0.1);
			glVertex3f((float)px, (float)py, -0.1);
		glEnd();
		DrawArc(ex, ey, knobradius, 0, 2*M_PI, 50, -0.1);
	} else if (vis == VIS_PIE) {
		glBegin(GL_LINES);
			glVertex3f((float)ex, (float)ey, -0.1);
			glVertex3f((float)hx, (float)hy, -0.1);
			glVertex3f((float)ex, (float)ey, -0.1);
			glVertex3f(ex+knobradius, (float)ey, -0.1);
		glEnd();
		float theta = atan2f(hy - ey, hx - ex);
		DrawArc(ex, ey, dist(ex, ey, hx, hy), 0, theta, 50, -0.1);
	}
}