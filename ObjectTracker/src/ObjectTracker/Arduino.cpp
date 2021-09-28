#include "stdafx.h"
#include "Arduino.h"

Arduino::Arduino(Console console) : console(console)
{
}

void Arduino::Ping()
{
	SendCommandWithAck("ping");
}

void Arduino::SendCommandWithAck(string command)
{
	console.Write(command);
	string response = console.Read();
	if (response.compare("Ack") != 0)
	{
		throw runtime_error("Command required 'Ack', but received '" + response + "'");
	}
}
