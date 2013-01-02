#include "Kiblet.h"
#include "ArcKidget.h"
#include "BackgroundKidget.h"
#include "BallKidget.h"
#include "BodyKidget.h"
#include "ClapKidget.h"
#include "HandsKidget.h"
#include "KibletKidget.h"
#include "PunchKidget.h"
#include "WaveKidget.h"

using namespace std;

Kiblet::Kiblet(void)
{
}


Kiblet::~Kiblet(void)
{
	for (int i = 0; i < kidgets.size(); ++i) {
		delete kidgets[i];
	}
}

Kidget* Kiblet::makeKidget(string type, Json::Value v) {
	if (type == "arc") return new ArcKidget(v);
	else if (type == "background") return new BackgroundKidget(v);
	else if (type == "ball") return new BallKidget(v);
	else if (type == "body") return new BodyKidget(v);
	else if (type == "clap") return new ClapKidget(v);
	else if (type == "hands") return new HandsKidget(v);
	else if (type == "kiblet") return new KibletKidget(v);
	else if (type == "punch") return new PunchKidget(v);
	else if (type == "wave") return new WaveKidget(v);
}

Kiblet::Kiblet(Json::Value v) {
	Json::Value::Members& members = v.getMemberNames();
	for (int i = 0; i < members.size(); ++i) {
		Kidget* k = makeKidget(members[i], v[members[i]]);
		kidgets.push_back(k);
	}
}

void Kiblet::sendOscMessages(UdpTransmitSocket& transmitSocket, const Vector4* skeletonPositions) {
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
	p << osc::BeginBundleImmediate;
	for (int i = 0; i < kidgets.size(); ++i) {
		kidgets[i]->sendOSC(p, skeletonPositions);
	}
	p << osc::EndBundle;
	transmitSocket.Send(p.Data(), p.Size());
}

bool Kiblet::checkTriggerCondition() {
	// Check time
	// Check OSC Messages
}