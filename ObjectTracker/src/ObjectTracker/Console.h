#pragma once

#include "SerialPort.h"
#include <tuple>
#define MAX_BUFFER_SIZE 50

using namespace std;

class Console
{
private:
	SerialPort serialPort;
	string stringSoFar;

	tuple<string, bool> SplitOnNewLine();
public:
	Console(SerialPort serialPort);
	
	void Write(string message);
	string Read();
};

