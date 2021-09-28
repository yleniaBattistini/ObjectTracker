#include "stdafx.h"
#include "Console.h"

Console::Console(SerialPort* serialPort) : serialPort(serialPort), readCharsSoFar("")
{
}

void Console::Write(string message)
{
	string messageWithNewLine = message + '\n';
	serialPort->WriteSerialPort(messageWithNewLine.c_str(), message.size());
}

tuple<string, bool> Console::SplitOnNewLine()
{
	const int newLinePosition = readCharsSoFar.find_first_of('\n');
	if (newLinePosition != string::npos)
	{
		string beforeNewLine = readCharsSoFar.substr(0, newLinePosition);
		string afterNewLine = readCharsSoFar.substr(newLinePosition + 1, readCharsSoFar.size() - 1 - newLinePosition);
		string result = readCharsSoFar + beforeNewLine;
		readCharsSoFar = afterNewLine;
		return make_tuple(result, true);
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
		serialPort->ReadSerialPort(buffer, MAX_BUFFER_SIZE - 1);
		buffer[MAX_BUFFER_SIZE - 1] = '\0';
		string readString = buffer;
		readCharsSoFar += readString;
		std::tie(s, isComplete) = SplitOnNewLine();
		if (!isComplete)
		{
			continue;
		}
		else
		{
			return s;
		}
	}
	return "";
}