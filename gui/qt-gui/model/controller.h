#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../serial/console.h"

#define MODE_AUTO 0
#define MODE_MANUAL 1

using namespace std;

class Controller
{
private:
    Console console;
    bool connected = false;

    string SendRequest(string request);
    void SendCommand(string command);
    void SendCommandWithAck(string command);
public:
    Controller(string portName);

    void Connect();
    void Disconnect();
    bool IsConnected();
    void SetMode(int mode);
    void SetDetected(bool detectedState);
    tuple<double, double> GetOffset();
};

#endif // CONTROLLER_H
