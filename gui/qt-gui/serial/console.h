#ifndef CONSOLE_H
#define CONSOLE_H

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

    tuple<string, bool> SplitOnNewLine();
public:
    Console(string portName);

    void Write(string message);
    string Read();
};


#endif // CONSOLE_H
