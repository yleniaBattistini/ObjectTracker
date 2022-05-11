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

void Controller::setDetectedState(bool detectedState)
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

void Controller::setCalibrationState(bool inCalibration)
{
    if (inCalibration)
    {
        sendCommand("calibration_on");
    }
    else
    {
        sendCommand("calibration_off");
    }
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
