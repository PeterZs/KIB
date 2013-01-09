#pragma once
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

class KinectHandler
{
public:
	KinectHandler(void);
	~KinectHandler(void);

	bool initSensors();
	void updateRgb(char* dest);
	void updateDepth(char* dest);
	void updateDepthAndPid(float* dest, char* pid);
	bool updateSkeleton();

	const Vector4* getSkeleton() const { return skeletonPosition; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	int width;
	int height;
	Vector4* skeletonPosition;

	HANDLE rgbstream, depthstream;
	HANDLE skeletonEvent;
	INuiSensor* sensor;
	bool initialized;
};

