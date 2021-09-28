#pragma once
#include <profileapi.h>

class StopWatch
{ 
public:
	void Start() 
	{ 
		QueryPerformanceCounter(&startTime);
	}

	double Stop() 
	{
		QueryPerformanceCounter(&stopTime);
		return __ElapsedTime();
	}
private:
	/// Get the time interval 
	double __ElapsedTime() 
	{ 
		LARGE_INTEGER timerFrequency;
		QueryPerformanceFrequency(&timerFrequency);
		__int64 oldTicks = ((__int64)startTime.HighPart << 32) + (__int64)startTime.LowPart;
		__int64 newTicks = ((__int64)stopTime.HighPart << 32) + (__int64)stopTime.LowPart; 
		long double timeDifference = (long double) (newTicks - oldTicks); 
		long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) + (__int64)timerFrequency.LowPart); 
		return (double)(timeDifference * 1000 / ticksPerSecond); 
	}
	LARGE_INTEGER startTime;
	LARGE_INTEGER stopTime;
};
