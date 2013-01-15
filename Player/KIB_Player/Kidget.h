#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "osc/OscOutboundPacketStream.h"
#include "json\json.h"
#include <gl\GLU.h>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI 3.1415926536
#include <exception>

class KIFormatException : public std::exception {
};

class Kidget {
public:
	Kidget(void) {;}
	~Kidget(void) {;}

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions) = 0;
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid) = 0;
	
	// From http://slabode.exofire.net/circle_draw.shtml
	void DrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments, float d=-0.01f) 
	{ 
		float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

		float tangetial_factor = tanf(theta);

		float radial_factor = cosf(theta);

	
		float x = r * cosf(start_angle);//we now start at the start angle
		float y = r * sinf(start_angle); 
    
		glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
		for(int ii = 0; ii < num_segments; ii++)
		{ 
			glVertex3f(x + cx, y + cy, d);

			float tx = -y; 
			float ty = x; 

			x += tx * tangetial_factor; 
			y += ty * tangetial_factor; 

			x *= radial_factor; 
			y *= radial_factor; 
		} 
		glEnd(); 
	}
	void fillArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments, float d=-0.01f, float colr=1.f, float colg=1.f, float colb=1.f, float cola=1.f) { 
		float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

		float tangetial_factor = tanf(theta);

		float radial_factor = cosf(theta);

	
		float x = r * cosf(start_angle);//we now start at the start angle
		float y = r * sinf(start_angle); 
    
		glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cx, cy, d);
        glColor4f(colr, colg, colb, cola);
		for(int ii = 0; ii < num_segments; ii++)
		{ 
			glVertex3f(x + cx, y + cy, d);

			float tx = -y; 
			float ty = x; 

			x += tx * tangetial_factor; 
			y += ty * tangetial_factor; 

			x *= radial_factor; 
			y *= radial_factor; 
		} 
		glEnd(); 
	}
protected:
	std::string oscname;

	inline bool isPositionValid(const Vector4& v) {
		return (v.z/v.w > 0); 
	}
};


inline double dist2(const Vector4& a, const Vector4& b) {
	double dx = a.x/a.w - b.x/b.w;
	double dy = a.y/a.w - b.y/b.w;
	double dz = a.z/a.w - b.z/b.w;
	return  dx*dx+dy*dy+dz*dz;
}
inline double dist(const Vector4& a, const Vector4& b) {
	return sqrt(dist2(a,b));
}
inline float dist2(float x1, float y1, float x2, float y2) {
	return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
}
inline float dist(float x1, float y1, float x2, float y2) {
	return sqrt(dist2(x1,y1,x2,y2));
}

inline double fRand(double fMin, double fMax)
{
    return fMin + ((double)rand() / RAND_MAX) * (fMax - fMin);
}
inline double randD() {
	return ((double)rand() / RAND_MAX);
}

inline int parseHex(const std::string& s) {
	int ret;
	sscanf_s(s.c_str(), "%x", &ret);
	return ret;
}
inline void parseColor(const std::string& s, unsigned char* rgb) {
	for (int i = 0; i < 3; ++i) {
		rgb[i] = (char) parseHex(s.substr(2*i+1,2));
	}
}
inline void drawLine(long* a, long* b, int aa, int bb, float d=-0.01f) {
	glVertex3f((float)a[aa], (float)b[aa], d);
	glVertex3f((float)a[bb], (float)b[bb], d);
}
