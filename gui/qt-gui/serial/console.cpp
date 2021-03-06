#include "console.h"

Console::Console(string portName) : serialPort(SerialPort(portName)), stringSoFar("")
{
}

void Console::write(string message)
{
    string messageWithNewLine = message + '\n';
    serialPort.writeSerialPort(messageWithNewLine.c_str(), message.size());
}

tuple<string, bool> Console::splitOnNewLine()
{
    const int newLinePosition = stringSoFar.find_first_of('\n');
    if (newLinePosition != string::npos)
    {
        string beforeNewLine = stringSoFar.substr(0, newLinePosition);
        string afterNewLine = stringSoFar.substr(newLinePosition + 1, stringSoFar.size() - 1 - newLinePosition);
        stringSoFar = afterNewLine;
        return make_tuple(beforeNewLine, true);
    }
    else
    {
        return make_tuple("", false);
    }
}

string Console::read()
{
    string s;
    bool isComplete;
    std::tie(s, isComplete) = splitOnNewLine();
    if (isComplete)
    {
        return s;
    }
    while (true)
    {
        char buffer[MAX_BUFFER_SIZE];
        const int readChars = serialPort.readSerialPort(buffer, MAX_BUFFER_SIZE - 1);
        buffer[readChars] = '\0';
        string readString = buffer;
        stringSoFar += readString;
        std::tie(s, isComplete) = splitOnNewLine();
        if (isComplete)
        {
            return s;
        }
    }
    return "";
}
