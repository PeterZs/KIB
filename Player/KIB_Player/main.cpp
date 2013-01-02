#include <Windows.h>
#include <iostream>
#include <fstream>
#include "json\json.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

// json, osc, kinect. opengl

int main(int argc, char* argv[]) {
	// Set up OSC communication ports
	char* address = "127.0.0.1";
	int port = 7000;
	int output_buffer_size = 1024;

	// Check arguments
	if (argc < 2 || argc > 3) {
		cout << "Usage: kibplayer filename.ki [port]" << endl;
		return 0;
	}
	// Parse JSON file
	ifstream in(argv[1]);
	if (!in) {
		cerr << "Error reading input file" << endl;
		return 0;
	}
	Json::Value root;
	Json::Reader reader;
	bool ret = reader.parse(in, root);
	if (!ret) {
		cerr << "Error parsing input file" << endl;
		return 0;
	}
	// Parse port
	if (argc == 3) {
		int argport = atoi(argv[2]);
		if (argport > 0) port = argport;
	}
	cout << "Root object has " << root.size() << " elements" << endl;
	UdpTransmitSocket transmitSocket(IpEndpointName(address, port));

}

/*
	App(transmitSocket, root);
	void update() {
		if (state == PLAYING) {
			handler.updateSkeleton();
			handler.updateRGB(rgbbuf);
			handler.updateDepth(depthbuf);
			if (nextKibletReady()) {
				kiblet = kiblets[curr+1];
			}
			kiblet.sendOscMessages(transmitSocket, handler.getSkeleton());
		} else if (state == PAUSE) {
		} else if (state == INIT) {
		}
	}
	void draw() {
		if (state == PLAYING || state == PAUSE) {
			kiblet.draw();
			// Draw menu?
		} else if (state == INIT) {
			// Draw menu
		}

	}
*/