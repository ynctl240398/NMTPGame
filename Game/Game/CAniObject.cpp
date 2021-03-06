#include "CAniObject.h"

CAniObject::CAniObject(D3DXVECTOR2 position, float vx, float vy, int aniId, D3DXVECTOR2 scale, long timeout, float gravity, int w, int h)
{
	this->_aniId = aniId;
	this->_scale = scale;
	this->_velocity = { vx, vy };
	this->_position = position;
	this->_ax = 0;
	this->_ay = gravity;
	this->timer.SetTimeOut(timeout);
	this->w = w;
	this->h = h;

	this->timer.Reset();
	this->timer.Start();
}

void CAniObject::Render()
{
	CGameObject::Render();
	//RenderBoundingBox();
}

void CAniObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;

	timer.Update(dt);

	if (timer.GetState() == CTimerState::TIMEOVER) {
		_isDeleted = true;
	}
}

void CAniObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x + w / 2;
	top = _position.y + h / 2;
	right = left + w;
	bottom = top + h;
}
