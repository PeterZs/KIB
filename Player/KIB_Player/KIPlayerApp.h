#pragma once
#include "app.h"
#include "KinectHandler.h"
#include "json\json.h"
#include "ip\UdpSocket.h"
#include <vector>
#include "Kiblet.h"

class KIPlayerApp :
	public App
{
public:
	KIPlayerApp(void);
	KIPlayerApp(Json::Value* root, UdpTransmitSocket* t);
	~KIPlayerApp(void);

	virtual bool onInit();
	virtual void onRender();
	virtual void onLoop();
private:
	bool initSDL();

	int curr;
	unsigned char* rgb;
	float* depth;
	char* pid;
	KinectHandler kinect;
	std::vector<Kiblet*> kiblets;
	UdpTransmitSocket* transmit;

	Json::Value* root;
};

