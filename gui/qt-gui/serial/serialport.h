#ifndef SERIALPORT_H
#define SERIALPORT_H


#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort(string portName);
    ~SerialPort();

    int ReadSerialPort(const char* buffer, unsigned int buf_size);
    bool WriteSerialPort(const char* buffer, unsigned int buf_size);
    bool IsConnected();
    void CloseSerial();
};

#endif // SERIALPORT_H
