#include "controller.h"
#include <string>

Controller::Controller(string portName) : console(Console(portName))
{
}

void Controller::Connect()
{
    if (connected)
    {
        throw std::runtime_error("Controller is already connected");
    }

    SendCommandWithAck("ping");
    connected = true;
}

void Controller::Disconnect()
{
    if (!connected)
    {
        throw std::runtime_error("Controller is not connected");
    }

    SendCommandWithAck("disconnect");
    connected = false;
}

bool Controller::IsConnected()
{
    return connected;
}

void Controller::SetMode(int mode)
{
    switch (mode)
    {
    case MODE_AUTO:
        SendCommand("mode_auto");
        break;
    case MODE_MANUAL:
        SendCommand("mode_manual");
        break;
    default:
        throw runtime_error("Unknown mode");
        break;
    }
}

void Controller::SetDetected(bool detectedState)
{
    if (detectedState)
    {
        SendCommand("detected");
    }
    else
    {
        SendCommand("not_detected");
    }
}

tuple<double, double> Controller::GetOffset()
{
    string response = SendRequest("offset");
    const int spacePos = response.find(' ');
    if (spacePos == string::npos)
    {
        throw runtime_error("Invalid offset format");
    }
    string xString = response.substr(0, spacePos);
    string yString = response.substr(spacePos + 1, response.size() - spacePos - 1);
    return make_tuple(stod(xString), stod(yString));
}

void Controller::SendCommandWithAck(string command)
{
    string response = SendRequest(command);
    if (response.compare("Ack") != 0)
    {
        throw runtime_error("Command required 'Ack', but received '" + response + "'");
    }
}

string Controller::SendRequest(string request)
{
    SendCommand(request);
    return console.Read();
}

void Controller::SendCommand(string command)
{
    console.Write(command);
}
