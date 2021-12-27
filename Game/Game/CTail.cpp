#include "CTail.h"
#include "CMario.h"
#include "CGoomba.h"

CTail::CTail()
{	
	_step = 0;
	_velocity = { 0, 0 };
	_liveTimer.Reset();
	_liveTimer.Start();
}

void CTail::Render()
{
	_aniId = 2000;
	//CGameObject::Render();
	RenderBoundingBox();
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();

	_position = mario->GetPosition();
	_position.y += 7;

	_liveTimer.Update(dt);

	if (_step == 0) {
		_velocity.x = (MARIO_TAIL_RANGE / dt) * mario->_direction * -1;
		if (_liveTimer.GetState() == CTimerState::TIMEOVER) {
			_step = 1;
			_liveTimer.Reset();
			_liveTimer.Start();
		}
	}
	else if (_step == 1) {
		_velocity.x = (MARIO_TAIL_RANGE / dt) * mario->_direction;
		if (_liveTimer.GetState() == CTimerState::TIMEOVER) {
			_step = 2;
			_liveTimer.Reset();
			_liveTimer.Start();
		}
	}
	else {
		_isDeleted = true;
	}
}

void CTail::OnNoCollision(DWORD dt)
{
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{	
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - MARIO_TAIL_BBOX_WIDTH / 2;
	top = _position.y - MARIO_TAIL_BBOX_HEIGHT / 2;
	right = left + MARIO_TAIL_BBOX_WIDTH;
	bottom = top + MARIO_TAIL_BBOX_HEIGHT;
}
