#include "KIPlayerApp.h"


KIPlayerApp::KIPlayerApp(void)
	: curr(0)
{
	running = true;
	screen = NULL;
	width=640;
	height=480;
	flags = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL;
}

KIPlayerApp::KIPlayerApp(Json::Value* rt, UdpTransmitSocket* t) 
	: transmit(t), curr(0), root(rt)
{

	running = true;
	screen = NULL;
	width=640;
	height=480;
	flags = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL;
}

KIPlayerApp::~KIPlayerApp(void)
{
	for (unsigned int i = 0; i < kiblets.size(); ++i) {
		delete kiblets[i];
	}
}

bool KIPlayerApp::initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	if (!info) return false;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
 
	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	screen = SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags);
	if (!screen) return false;
	else return true;
}

bool KIPlayerApp::onInit() {
	if (!initSDL()) return false;
	kinect = KinectHandler();
	for (int i = 0; i < 100; ++i) {
		if (kinect.initSensors()) break;
		Sleep(100);
	}
	if (!kinect.initSensors()) return false;
	//NuiCameraElevationSetAngle(10);
	depth = new float[kinect.getHeight()*kinect.getWidth()];
	pid = new char[kinect.getHeight()*kinect.getWidth()];
	rgb = new unsigned char[kinect.getHeight()*kinect.getWidth()*4];
	
	for (unsigned int i = 0; i < root->size(); ++i) {
		kiblets.push_back(new Kiblet((*root)[i]));
	}
	return true;
}

void KIPlayerApp::onLoop() {
	kinect.updateSkeleton();
	kinect.updateDepthAndPid(depth, pid);
	kinect.updateRgb((char*) rgb);
	kiblets[curr]->sendOscMessages(transmit, kinect.getSkeleton());
}

void KIPlayerApp::onRender() {
	kiblets[curr]->draw(kinect.getSkeleton(), rgb, depth, pid, kinect.getWidth(), kinect.getHeight());
	SDL_GL_SwapBuffers();
}