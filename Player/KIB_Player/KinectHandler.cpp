#include "KinectHandler.h"
#include <cstdio>


KinectHandler::KinectHandler(void)
{
	width=640;
	height=480;
	skeletonPosition = new Vector4[NUI_SKELETON_POSITION_COUNT];
	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) skeletonPosition[i] = Vector4();
	initialized = false;
}


KinectHandler::~KinectHandler(void)
{
	delete [] skeletonPosition;
}

bool KinectHandler::initSensors() {
	if (initialized) return true;
	int numSensors;
	long result = -1;

	if (NuiGetSensorCount(&numSensors) < 0) return false;

	for (int i = 0; i < numSensors; ++i) {
		if (NuiCreateSensorByIndex(i, &sensor) < 0) continue;
		if (sensor->NuiStatus() == 0) break;
		sensor->Release();
	}
	if (sensor) {
		if (sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON) == 0) {
			result = sensor->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,			 // Image type
				NUI_IMAGE_RESOLUTION_640x480,	 // Resolution
				0,								 // Image stream flags, e.g. near mode, player indices (?) min/max depth (?)
				//NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,
				2,								 // Number of frames to buffer
				NULL,
				&depthstream);
			if (result < 0) return false;
			result = sensor->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_COLOR,			 // Image type
				NUI_IMAGE_RESOLUTION_640x480,	 // Resolution
				0,								 // Image stream flags, e.g. near mode, player indices (?) min/max depth (?)
				2,								 // Number of frames to buffer
				NULL,
				&rgbstream);
			if (result < 0) return false;
			skeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
			result = sensor->NuiSkeletonTrackingEnable(skeletonEvent, 0);
		}
	}
	if (result < 0 || !sensor) return false;
	initialized = true;
	return true;
}
void KinectHandler::updateRgb(char* dest) {
	if (!sensor || !initialized) return;
	static NUI_IMAGE_FRAME imageFrame;
	static NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(rgbstream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    texture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        char* start = (char*) LockedRect.pBits;
        char* dataEnd = start + (width*height*4);
		char* to = dest;
		char* from = start;

		while (from != dataEnd) {
			*to++ = *from++;
		}
    }
    // We're done with the texture so unlock it
    texture->UnlockRect(0);
    // Release the frame
    sensor->NuiImageStreamReleaseFrame(rgbstream, &imageFrame);
}
void KinectHandler::updateDepth(char* dest) {
	if (!sensor || !initialized) return;
	static NUI_IMAGE_FRAME imageFrame;
	static NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(depthstream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    texture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        char* start = (char*) LockedRect.pBits;
        char* dataEnd = start + (width*height*2);
		char* to = dest;
		char* from = start;

		while (from != dataEnd) {
			*(to++) = *(from++);
		}
    }
    // We're done with the texture so unlock it
    texture->UnlockRect(0);
    // Release the frame
    sensor->NuiImageStreamReleaseFrame(depthstream, &imageFrame);
}
void KinectHandler::updateDepthAndPid(float* dest, char* pid) {
	if (!sensor || !initialized) return;
	static NUI_IMAGE_FRAME imageFrame;
	static NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(depthstream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    texture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        const USHORT* curr = (const USHORT*) LockedRect.pBits;
        const USHORT* dataEnd = curr + (width*height);

        while (curr < dataEnd) {
			*dest++ = NuiDepthPixelToDepth(*curr)/1000.f;
			*pid++ = NuiDepthPixelToPlayerIndex(*curr);
			++curr;
        }
    }
    // We're done with the texture so unlock it
    texture->UnlockRect(0);
    // Release the frame
    sensor->NuiImageStreamReleaseFrame(depthstream, &imageFrame);
}

bool KinectHandler::updateSkeleton() {
	if (WAIT_OBJECT_0 == WaitForSingleObject(skeletonEvent, 0)) {
        NUI_SKELETON_FRAME skeletonFrame = {0};
        if (sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame) >= 0) {
			sensor->NuiTransformSmooth(&skeletonFrame, NULL);
			for (int z = 0; z < NUI_SKELETON_COUNT; ++z) {
				const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[z];
				if (skeleton.eTrackingState == NUI_SKELETON_TRACKED) {			
					for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) {
						skeletonPosition[i] = skeleton.SkeletonPositions[i];
						if (skeleton.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_NOT_TRACKED) {
							skeletonPosition[i].z = -1; skeletonPosition[i].w = 1;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}