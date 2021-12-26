#include "CTimer.h"

CTimer::CTimer(bool useGameTime, long timeout) {
	this->useGameTime = useGameTime;
	this->timeleft = this->timeout = timeout;
	this->started = false;
	this->startPoint = GetTickCount64();
}

void CTimer::SetTimeOut(long timeout) {
	this->timeleft = this->timeout = timeout;
}

void CTimer::AddTimeOut(long timeout) {
	this->timeleft += timeout;
	this->timeout += timeout;
}

bool CTimer::IsTimeOver() {
	return this->timeleft <= 0;
}

bool CTimer::IsRunning() {
	return started;
}

void CTimer::Start() {
	this->started = true;
	this->startPoint = GetTickCount64();
}

void CTimer::Stop() {
	this->started = false;
}

void CTimer::Reset() {
	this->started = false;
	this->timeleft = timeout;
}

void CTimer::Update(unsigned long dt) {
	if (!started) return;

	if (useGameTime) {
		this->timeleft -= dt;
	}
	else {
		this->timeleft = timeout - (GetTickCount64() - startPoint);
	}

	if (timeleft <= 0) Stop();
}
