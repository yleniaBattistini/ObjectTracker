#pragma once
#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <iostream>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort(const char* portName, DWORD baudRate);
    ~SerialPort();

    int ReadSerialPort(const char* buffer, unsigned int buf_size);
    bool WriteSerialPort(const char* buffer, unsigned int buf_size);
    bool IsConnected();
    void CloseSerial();
};
