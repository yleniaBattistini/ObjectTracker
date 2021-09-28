#pragma once
#include "Console.h"

using namespace std;

class Arduino
{
private:
	Console console;

	void SendCommandWithAck(string command);
public:
	Arduino(Console console);

	void Ping();
};

