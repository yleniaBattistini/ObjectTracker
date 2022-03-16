#ifndef SERIALPORTSHELPER_H
#define SERIALPORTSHELPER_H

#include <string>
#include <vector>

using namespace std;

class SerialPortsHelper
{
public:
    static vector<string> ListAvailablePorts();
};

#endif // SERIALPORTSHELPER_H
