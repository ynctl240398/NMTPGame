#pragma once
#include "CUtil.h"

enum class CTimerState
{
	RUNNING,
	TIMEOVER,
	STOPPED
};

class CTimer
{
private:
	bool useGameTime;

	CTimerState state;

	ULONGLONG timeleft;

	ULONGLONG timeout;

	ULONGLONG startPoint;

public:
	CTimer(bool useGameTime, long timeout = 0);

	void SetTimeOut(long timeout);

	void AddTimeOut(long timeout);

	CTimerState GetState();

	void Start();

	void Stop();

	void Reset();

	void Update(unsigned long dt);

};

