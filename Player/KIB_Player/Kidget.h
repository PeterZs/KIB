#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "osc/OscOutboundPacketStream.h"
#include "json\json.h"
#include <gl\GLU.h>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>

#include <exception>

class KIFormatException : public std::exception {
};

class Kidget {
public:
	Kidget(void) {;}
	~Kidget(void) {;}

	virtual void sendOSC(osc::OutboundPacketStream& p, const Vector4* skeletonPositions) = 0;
	virtual void draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid) = 0;

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

inline double fRand(double fMin, double fMax)
{
    return fMin + ((double)rand() / RAND_MAX) * (fMax - fMin);
}
inline double randD() {
	return ((double)rand() / RAND_MAX);
}