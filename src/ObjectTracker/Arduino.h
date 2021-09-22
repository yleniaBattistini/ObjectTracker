#pragma once
class Arduino
{
public:
	void Connect();
	void Disconnect();
	bool IsConnected();

	void SetObjectRecognized(bool recognizedState);
};

