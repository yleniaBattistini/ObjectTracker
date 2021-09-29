#include "stdafx.h"
#include "Console.h"

Console::Console(const char* portName) : serialPort(SerialPort(portName)), stringSoFar("")
{
}

void Console::Write(string message)
{
	string messageWithNewLine = message + '\n';
	serialPort.WriteSerialPort(messageWithNewLine.c_str(), message.size());
}

tuple<string, bool> Console::SplitOnNewLine()
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

string Console::Read()
{
	string s;
	bool isComplete;
	std::tie(s, isComplete) = SplitOnNewLine();
	if (isComplete)
	{
		return s;
	}
	while (true)
	{
		char buffer[MAX_BUFFER_SIZE];
		const int readChars = serialPort.ReadSerialPort(buffer, MAX_BUFFER_SIZE - 1);
		buffer[readChars] = '\0';
		string readString = buffer;
		stringSoFar += readString;
		std::tie(s, isComplete) = SplitOnNewLine();
		if (isComplete)
		{
			return s;
		}
	}
	return "";
}