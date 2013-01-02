#pragma once
#include "Kidget.h"
#include <vector>
#include <string>
#include "ip/UdpSocket.h"
#define OUTPUT_BUFFER_SIZE 1024

class Kiblet
{
public:
	Kiblet(void);
	~Kiblet(void);
	Kiblet(Json::Value v);

	void draw();
	void sendOscMessages(UdpTransmitSocket& transmitSocket, const Vector4* skeletonPositions);
	static Kidget* makeKidget(std::string type, Json::Value v);
	bool checkTriggerCondition();
private:
	std::vector<Kidget*> kidgets;
	char buffer[OUTPUT_BUFFER_SIZE];
};

