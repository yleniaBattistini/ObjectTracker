#pragma once

#include "SerialPort.h"
#include <tuple>
#define MAX_BUFFER_SIZE 50

using namespace std;

class Console
{
private:
	SerialPort* serialPort;
	string readCharsSoFar;

	tuple<string, bool> SplitOnNewLine();
public:
	Console(SerialPort* serialPort);
	~Console();
	
	void Write(string message);
	string Read();
};

