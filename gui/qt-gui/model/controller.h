#pragma once

#include "../serial/console.h"

#define MODE_AUTO 0
#define MODE_MANUAL 1

using namespace std;

class Controller
{
private:
    Console console;
    bool connected = false;

    string sendRequest(string request);
    void sendCommand(string command);
    void sendCommandWithAck(string command);
public:
    Controller(string portName);

    void connect();
    void disconnect();
    bool isConnected();
    void setDetectedState(bool detectedState);
    void setCalibrationState(bool inCalibration);
};
