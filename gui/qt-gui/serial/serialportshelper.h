#pragma once

#include <string>
#include <vector>

using namespace std;

class SerialPortsHelper
{
public:
    static vector<string> listAvailablePorts();
};
