#pragma once

#include "serialport.h"
#include <tuple>
#include <string>
#define MAX_BUFFER_SIZE 50

using namespace std;

class Console
{
private:
    SerialPort serialPort;
    string stringSoFar;

    tuple<string, bool> splitOnNewLine();
public:
    Console(string portName);

    void write(string message);
    string read();
};
