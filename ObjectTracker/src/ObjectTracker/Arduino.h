#pragma once
#include "Console.h"

#define MODE_AUTO 0
#define MODE_MANUAL 1

using namespace std;

class Arduino
{
private:
	Console console;

	string SendRequest(string request);
	void SendCommand(string command);
	void SendCommandWithAck(string command);
public:
	Arduino(char *portName);

	void Connect();
	void Disconnect();
	void SetMode(int mode);
	void SetDetected(bool detectedState);
	tuple<double, double> GetOffset();
};

