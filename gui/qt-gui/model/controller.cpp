#include "controller.h"
#include <string>

Controller::Controller(string portName) : console(Console(portName))
{
}

void Controller::connect()
{
    if (connected)
    {
        throw std::runtime_error("Controller is already connected");
    }

    sendCommandWithAck("ping");
    connected = true;
}

void Controller::disconnect()
{
    if (!connected)
    {
        throw std::runtime_error("Controller is not connected");
    }

    sendCommandWithAck("disconnect");
    connected = false;
}

bool Controller::isConnected()
{
    return connected;
}

void Controller::setMode(int mode)
{
    switch (mode)
    {
    case MODE_AUTO:
        sendCommand("mode_auto");
        break;
    case MODE_MANUAL:
        sendCommand("mode_manual");
        break;
    default:
        throw runtime_error("Unknown mode");
        break;
    }
}

void Controller::setDetected(bool detectedState)
{
    if (detectedState)
    {
        sendCommand("detected");
    }
    else
    {
        sendCommand("not_detected");
    }
}

tuple<double, double> Controller::getOffset()
{
    string response = sendRequest("offset");
    const int spacePos = response.find(' ');
    if (spacePos == string::npos)
    {
        throw runtime_error("Invalid offset format");
    }
    string xString = response.substr(0, spacePos);
    string yString = response.substr(spacePos + 1, response.size() - spacePos - 1);
    return make_tuple(stod(xString), stod(yString));
}

void Controller::sendCommandWithAck(string command)
{
    string response = sendRequest(command);
    if (response.compare("Ack") != 0)
    {
        throw runtime_error("Command required 'Ack', but received '" + response + "'");
    }
}

string Controller::sendRequest(string request)
{
    sendCommand(request);
    return console.read();
}

void Controller::sendCommand(string command)
{
    console.write(command);
}
