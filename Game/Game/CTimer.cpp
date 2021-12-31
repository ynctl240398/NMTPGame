#include "CTimer.h"

CTimer::CTimer(bool useGameTime, long timeout) {
	this->useGameTime = useGameTime;
	this->timeleft = this->timeout = timeout;
	this->state = CTimerState::STOPPED;
	this->startPoint = GetTickCount64();
}

void CTimer::SetTimeOut(long timeout) {
	AddTimeOut(timeout - (long)this->timeout);
}

void CTimer::AddTimeOut(long timeout) {
	this->timeleft += timeout;
	this->timeout += timeout;
}

CTimerState CTimer::GetState()
{
	return state;
}

void CTimer::Start() {
	this->state = CTimerState::RUNNING;
	this->startPoint = GetTickCount64();
}

void CTimer::Stop() {
	this->state = CTimerState::STOPPED;
}

void CTimer::Reset() {
	this->state = CTimerState::STOPPED;
	this->timeleft = timeout;
}

void CTimer::Update(unsigned long dt) {
	if (state != CTimerState::RUNNING) return;

	if (useGameTime) {
		this->timeleft -= dt;
	}
	else {
		this->timeleft = timeout - (GetTickCount64() - startPoint);
	}

	if (timeleft < 0) {
		state = CTimerState::TIMEOVER;
		timeleft = 0;
	}
}
