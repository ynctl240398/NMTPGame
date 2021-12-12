#include "CPiranhaPlant.h"
#include "CAnimation.h"

#define ID_ANI_PIRANHA_PLANT_RED_UP		9000
#define ID_ANI_PIRANHA_PLANT_GREEN_UP	9001

#define PIRANHA_PLANT_SPEED_Y			0.02f
#define TIME_TO_UP						2000

int CPiranhaPlant::_GetAnimationId()
{
	int aniId = -1;
	if (_state == STATE_PIRANHA_PLANT_UP) {
		aniId = _type == TYPE_PIRANHA_PLANT_RED ? ID_ANI_PIRANHA_PLANT_RED_UP : ID_ANI_PIRANHA_PLANT_GREEN_UP;
	}
	return aniId;
}

CPiranhaPlant::CPiranhaPlant(float x, float y, int type, float offSetY)
{
	_position = { x,y + PIRANHA_PLANT_BBOX_HIEGHT };
	_offSetY = offSetY;
	_startPostion = _position;
	_startTime = 0;
	_type = type;
	_isUp = true;
	SetState(STATE_PIRANHA_PLANT_IDLE);
}

void CPiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case STATE_PIRANHA_PLANT_IDLE:
		break;
	case STATE_PIRANHA_PLANT_UP:
		break;
	case STATE_PIRANHA_PLANT_DIE:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CPiranhaPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetBackBufferWidth() + leftCam;

	if (_position.x >= leftCam && _position.x <= rightCam) {
		if (_state == STATE_PIRANHA_PLANT_IDLE) {
			if (_startTime == 0) {
				_startTime = GetTickCount64();
			}
			else if (GetTickCount64() - _startTime > TIME_TO_UP) {
				SetState(STATE_PIRANHA_PLANT_UP);
				_startTime = 0;
			}
		}
		else if (_state == STATE_PIRANHA_PLANT_UP) {
			if (_isUp) {
				if (_startPostion.y - _position.y < _offSetY) {
					_velocity.y = -PIRANHA_PLANT_SPEED_Y;
				}
				else {
					_velocity.y = 0;
					if (_startTime == 0) {
						_startTime = GetTickCount64();
					}
					else if (GetTickCount64() - _startTime > TIME_TO_UP) {
						_startTime = 0;
						_isUp = false;
					}
				}
			}
			else {
				if (_startPostion.y >= _position.y) {
					_velocity.y = PIRANHA_PLANT_SPEED_Y;
				}
				else {
					_velocity.y = 0;
					SetState(STATE_PIRANHA_PLANT_IDLE);
					_isUp = true;
				}
			}
		}
	}

	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
}

void CPiranhaPlant::OnNoCollision(DWORD dt)
{
}

void CPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_state != STATE_PIRANHA_PLANT_IDLE || _state != STATE_PIRANHA_PLANT_DIE) {
		left = _position.x - PIRANHA_PLANT_BBOX_WIDTH / 2;
		top = _position.y - PIRANHA_PLANT_BBOX_HIEGHT / 2;
		right = left + PIRANHA_PLANT_BBOX_WIDTH;
		bottom = top + PIRANHA_PLANT_BBOX_HIEGHT;
	}
}
