#pragma once
#include "CUtil.h"

class CTimer
{
private:
	bool useGameTime;

	bool started;

	long timeleft;

	long timeout;

	long startPoint;

public:
	CTimer(bool useGameTime, long timeout = 0);

	void SetTimeOut(long timeout);

	void AddTimeOut(long timeout);

	bool IsTimeOver();

	bool IsRunning();

	void Start();

	void Stop();

	void Reset();

	void Update(unsigned long dt);

};

