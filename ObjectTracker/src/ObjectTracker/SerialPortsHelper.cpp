#include "stdafx.h"
#include "SerialPortsHelper.h"
#include <Windows.h>

#define PORT_MAX 128
#define PATH_MAX 500

vector<string> SerialPortsHelper::ListAvailablePorts()
{
	vector<string> ports;
	for (int i = 0; i < PORT_MAX; i++)
	{
		char path[PATH_MAX];
		string name = "COM" + to_string(i);
		DWORD test = QueryDosDevice(name.c_str(), path, PATH_MAX);
		if (test != 0)
		{
			ports.push_back(name);
		}

	}
	return ports;
}