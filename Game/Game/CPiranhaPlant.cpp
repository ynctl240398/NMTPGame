#include "CPiranhaPlant.h"
#include "CAnimation.h"
#include "CKoopaParaTropa.h"
#include "CKoopaTropa.h"
#include "CTail.h"

#define ID_ANI_PIRANHA_PLANT_RED_UP		9000
#define ID_ANI_PIRANHA_PLANT_GREEN_UP	9001

#define PIRANHA_PLANT_SPEED_Y			0.02f

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
	_position = { x,y + PIRANHA_PLANT_BBOX_HIEGHT + 2 };
	_offSetY = offSetY;
	_startPostion = _position;
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
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetBackBufferWidth() + leftCam;

	_startTimer.Update(dt);

	if (_position.x >= leftCam && _position.x <= rightCam) {
		if (_state == STATE_PIRANHA_PLANT_IDLE) {
			if (_startTimer.GetState() != CTimerState::RUNNING) {
				SetState(STATE_PIRANHA_PLANT_UP);
				_startTimer.Reset();
				_startTimer.Start();
			}
		}
		else if (_state == STATE_PIRANHA_PLANT_UP) {
			if (_isUp) {
				if (_startPostion.y - _position.y < _offSetY) {
					_velocity.y = -PIRANHA_PLANT_SPEED_Y;
				}
				else {
					_velocity.y = 0;
					if (_startTimer.GetState() == CTimerState::TIMEOVER) {
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

					_startTimer.Reset();
					_startTimer.Start();
				}
			}
		}

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CPiranhaPlant::OnNoCollision(DWORD dt)
{
}

void CPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CKoopaParaTropa* paraKoopa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (paraKoopa) {
		if (paraKoopa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
			_isDeleted = true;
		}
	}

	CKoopaTropa* koopa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (koopa) {
		if (koopa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN) {
			_isDeleted = true;
		}
	}

	CTail* tail = dynamic_cast<CTail*>(e->obj);
	if (tail) {
		_isDeleted = true;
	}
}

int CPiranhaPlant::IsBlocking(LPCOLLISIONEVENT e)
{
	return 0;
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
