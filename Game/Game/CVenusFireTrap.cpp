#include "CVenusFireTrap.h"
#include "CAnimation.h"
#include "CMario.h"

#define ID_ANI_VENUS_FIRE_TRAP_RED_UP			8000
#define ID_ANI_VENUS_FIRE_TRAP_RED_UP_FIRER		8001
#define ID_ANI_VENUS_FIRE_TRAP_RED_DOWN			8002
#define ID_ANI_VENUS_FIRE_TRAP_RED_DOWN_FIRER	8003
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_UP			8004
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_UP_FIRER	8005
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN		8006
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN_FIRER	8007

#define TIME_TO_FIRE							2000 //s
#define VENUS_SPEED_Y							0.02f

CVenusFireTrap::CVenusFireTrap(float x, float y, int type, float offSetY)
{
	_position = { x,y + VENUS_FIRE_TRAP_BBOX_HIEGHT };
	_offSetY = offSetY;
	_startPostion = _position;
	_startTime = 0;
	_type = type;
	_isUp = true;
	SetState(STATE_VENUS_FIRE_TRAP_IDLE);
	_firer = NULL;
}

void CVenusFireTrap::SetState(int state)
{
	switch (state)
	{
	case STATE_VENUS_FIRE_TRAP_UP_FIRER:
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
		break;
	case STATE_VENUS_FIRE_TRAP_UP:
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN:
		break;
	case STATE_VENUS_FIRE_TRAP_IDLE:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CVenusFireTrap::Render()
{
	if (_firer != NULL) {
		_firer->Render();
	}

	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

int CVenusFireTrap::_GetAnimationId()
{
	int aniId = -1;

	switch (_state)
	{
	case STATE_VENUS_FIRE_TRAP_UP_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_UP_FIRER : ID_ANI_VENUS_FIRE_TRAP_GREEN_UP_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_DOWN_FIRER : ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_UP:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_UP : ID_ANI_VENUS_FIRE_TRAP_GREEN_UP;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_DOWN : ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN;
		break;
	default:
		break;
	}

	return aniId;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetBackBufferWidth() + leftCam;

	if (_position.x >= leftCam && _position.x <= rightCam) {
		CMario* mario = CMario::GetInstance();
		if (mario->GetPosition().x < _position.x) {
			_scale = { 1.0f,1.0f };
		}
		else _scale = { -1.0f,1.0f };

		if (_state == STATE_VENUS_FIRE_TRAP_IDLE) {
			if (_startTime == 0) {
				_startTime = GetTickCount64();
			}
			else if (GetTickCount64() - _startTime > TIME_TO_FIRE) {
				SetState(STATE_VENUS_FIRE_TRAP_UP);
				_startTime = 0;
			}
		}
		else if (_state == STATE_VENUS_FIRE_TRAP_UP) {
			if (_isUp) {
				if (_startPostion.y - _position.y < _offSetY) {
					_velocity.y = -VENUS_SPEED_Y;
				}
				else {
					_velocity.y = 0;
					if (mario->GetPosition().y < _position.y) {
						SetState(STATE_VENUS_FIRE_TRAP_UP_FIRER);
					}
					else SetState(STATE_VENUS_FIRE_TRAP_DOWN_FIRER);
					_startTime = 0;
				}
			}
			else {
				if (_startPostion.y >= _position.y) {
					_velocity.y = VENUS_SPEED_Y;
				}
				else {
					_velocity.y = 0;
					SetState(STATE_VENUS_FIRE_TRAP_IDLE);
					_isUp = true;
				}
			}
		}
		else if (_state == STATE_VENUS_FIRE_TRAP_UP_FIRER || _state == STATE_VENUS_FIRE_TRAP_DOWN_FIRER) {
			if (_startTime == 0) {
				_startTime = GetTickCount64();
			}
			else if (GetTickCount64() - _startTime == TIME_TO_FIRE / 2) {
				_firer = new CFirer(_position.x, _position.y - DIF);
				_firer->SetState(STATE_FIRER_FLY);
			}
			else if (GetTickCount64() - _startTime > TIME_TO_FIRE) {
				SetState(STATE_VENUS_FIRE_TRAP_UP);
				_startTime = 0;
				_isUp = false;
			}
		}
	}

	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;

	if (_firer != NULL) {
		vector<LPGAMEOBJECT> objs;
		objs.push_back(CMario::GetInstance());
		_firer->Update(dt, &objs);
	}
}

void CVenusFireTrap::Release()
{
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
}

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_state != STATE_VENUS_FIRE_TRAP_DIE || _state != STATE_VENUS_FIRE_TRAP_IDLE) {
		left = _position.x - VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
		top = _position.y - VENUS_FIRE_TRAP_BBOX_HIEGHT / 2;
		right = left + VENUS_FIRE_TRAP_BBOX_WIDTH;
		bottom = top + VENUS_FIRE_TRAP_BBOX_HIEGHT;
	}
}
