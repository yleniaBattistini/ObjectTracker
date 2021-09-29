#include "stdafx.h"
#include "Arduino.h"
#include <string>

Arduino::Arduino(char* portName) : console(Console(portName))
{
}

void Arduino::Connect()
{
	SendCommandWithAck("ping");
}

void Arduino::Disconnect()
{
	SendCommandWithAck("disconnect");
}

void Arduino::SetMode(int mode)
{
	switch (mode)
	{
	case MODE_AUTO:
		SendCommand("mode_auto");
		break;
	case MODE_MANUAL:
		SendCommand("mode_manual");
		break;
	default:
		throw runtime_error("Unknown mode");
		break;
	}
}

void Arduino::SetDetected(bool detectedState)
{
	if (detectedState)
	{
		SendCommand("detected");
	}
	else
	{
		SendCommand("not_detected");
	}
}

tuple<double, double> Arduino::GetOffset()
{
	string response = SendRequest("offset");
	const int spacePos = response.find(' ');
	if (spacePos == string::npos)
	{
		throw runtime_error("Invalid offset format");
	}
	string xString = response.substr(0, spacePos);
	string yString = response.substr(spacePos + 1, response.size() - spacePos - 1);
	return make_tuple(stod(xString), stod(yString));
}

void Arduino::SendCommandWithAck(string command)
{
	string response = SendRequest(command);
	if (response.compare("Ack") != 0)
	{
		throw runtime_error("Command required 'Ack', but received '" + response + "'");
	}
}

string Arduino::SendRequest(string request)
{
	SendCommand(request);
	return console.Read();
}

void Arduino::SendCommand(string command)
{
	console.Write(command);
}
