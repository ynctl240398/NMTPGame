#include "CParaGoomba.h"
#include "CAnimation.h"
#include "CBrickQuestion.h"
#include "CBrick.h"
#include "CItem.h"
#include "CGoomba.h"
#include "CDebug.h"
#include "CMario.h"
#include "CKoopaParaTropa.h"
#include "CKoopaTropa.h"
#include "CTail.h"
#include "CAniObject.h"

#define PARA_GOOMBA_BBOX_WIDTH 20
#define PARA_GOOMBA_BBOX_HEIGHT 24

#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 16

#define ID_PARA_GOOMBA_WALK_ANI 5000
#define ID_PARA_GOOMBA_FLY_ANI 5001
#define ID_RED_GOOMBA_WALK_ANI 5002
#define ID_RED_GOOMBA_DIE_ANI 5003

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_GRAVITY 0.0008f
#define GOOMBA_WALK_SPEED 0.04f
#define PARA_GOOMBA_WALK_SPEED 0.02f
#define PARA_GOOMBA_FLY_SPEED 0.3f
#define PARA_GOOMBA_FLY_SPEED_SHORT 0.18f


void CParaGoomba::_Die(LPCOLLISIONEVENT e)
{
	_isDeleted = true;

	CAniObject* aniObj = new CAniObject({ _position.x, _position.y + 4 }, 0, 0, ID_RED_GOOMBA_DIE_ANI, { 1, 1 }, GOOMBA_DIE_TIMEOUT, 0.0f);
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
}

void CParaGoomba::_DieJump(LPCOLLISIONEVENT e)
{
	_isDeleted = true;

	CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.25f, _GetAnimationId(), { 1, -1 });
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
}

CParaGoomba::CParaGoomba(float x, float y) {
	_position = { x,y };
	_ax = 0;
	_ay = GOOMBA_GRAVITY;
	_countJump = 0;
	_velocity = { 0.0f,0.0f };
	_state = STATE_PARA_GOOMBA_IDLE;
}

int CParaGoomba::_GetAnimationId() {
	int aniId = -1;
	switch (_state)
	{
	case STATE_RED_GOOMBA_WALK:
		aniId = ID_RED_GOOMBA_WALK_ANI;
		break;
	case STATE_PARA_GOOMBA_FLY:
		aniId = ID_PARA_GOOMBA_FLY_ANI;
		break;
	case STATE_PARA_GOOMBA_WALK:
		aniId = ID_PARA_GOOMBA_WALK_ANI;
		break;
	default:
		aniId = -1;
		break;
	}
	return aniId;
}

void CParaGoomba::SetState(int state) {
	float l, t, r, b;
	float nl, nt, nr, nb;
	GetBoundingBox(l, t, r, b);

	_ay = GOOMBA_GRAVITY;

	float sign = _velocity.x >= 0 ? 1 : -1;

	switch (state)
	{
	case STATE_PARA_GOOMBA_WALK:
		_velocity = { PARA_GOOMBA_WALK_SPEED, 0 };
		break;
	case STATE_PARA_GOOMBA_FLY:
		_countJump = 0;
		_velocity = { PARA_GOOMBA_WALK_SPEED, -PARA_GOOMBA_FLY_SPEED_SHORT };
		break;
	case STATE_RED_GOOMBA_WALK:
		_velocity = { GOOMBA_WALK_SPEED, 0 };
		break;
	default:
		break;
	}
	CGameObject::SetState(state);

	GetBoundingBox(nl, nt, nr, nb);
	_position.y -= (nb - b);
	_velocity.x *= sign;
}

void CParaGoomba::OnNoCollision(DWORD dt) {
	_position -= _velocity * dt;
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt / 2;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	switch (_state)
	{
	case STATE_PARA_GOOMBA_FLY:
	case STATE_PARA_GOOMBA_WALK:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_RED_GOOMBA_WALK);
			}
		}
		break;
	case STATE_RED_GOOMBA_WALK:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				_Die(e);
			}
		}
		break;
	default:
		break;
	}

	CKoopaParaTropa* paraKoopa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (paraKoopa) {
		if (paraKoopa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == paraKoopa) {
			_DieJump(e);
		}
	}

	CKoopaTropa* koopa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (koopa) {
		if (koopa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == koopa) {
			_DieJump(e);
		}
	}

	if (dynamic_cast<CTail*>(e->obj)) {
		_DieJump(e);
	}
}

void CParaGoomba::OnBlockingOn(bool isHorizontal, float z)
{
	if (isHorizontal) {
		_velocity.x *= -1;
	}
	else {
		if (_state == STATE_PARA_GOOMBA_FLY && z < 0) {
			_countJump++;
			if (_countJump == 1) {
				_velocity.y = -PARA_GOOMBA_FLY_SPEED;
			}
			if (_countJump == 2) {
				SetState(STATE_PARA_GOOMBA_WALK);
			}
		}
		else {
			_velocity.y = 0;
		}
	}
}

int CParaGoomba::IsBlocking(LPCOLLISIONEVENT e)
{
	if (e->obj == CMario::GetInstance()) {
		if (e->ny > 0) {
			return true;
		}
	}
	return dynamic_cast<CParaGoomba*>(e->obj) == nullptr;
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	_walkTimer.Update(dt);

	if (_state == STATE_PARA_GOOMBA_IDLE) {
		if (_position.x <= rightCam && _position.x >= leftCam) {
			SetState(STATE_PARA_GOOMBA_WALK);
		}
	}
	else {
		switch (_state)
		{
		case STATE_PARA_GOOMBA_WALK:
			if (_walkTimer.GetState() == CTimerState::STOPPED) {
				if ((_position.x - CMario::GetInstance()->GetPosition().x) * _velocity.x >= 0) {
					_velocity.x *= -1;
				}
				_walkTimer.Reset();
				_walkTimer.Start();
			}
			if (_walkTimer.GetState() == CTimerState::TIMEOVER) {
				SetState(STATE_PARA_GOOMBA_FLY);
				_walkTimer.Stop();
			}
			break;
		case STATE_PARA_GOOMBA_FLY:
			if (_countJump == 0) {
				_velocity.y = max(_velocity.y, -PARA_GOOMBA_FLY_SPEED_SHORT);
			}
			if (_countJump == 1) {
				_velocity.y = max(_velocity.y, -PARA_GOOMBA_FLY_SPEED);
			}
			break;
		case STATE_RED_GOOMBA_WALK:
			break;
		default:
			break;
		}

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CParaGoomba::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_state <= STATE_PARA_GOOMBA_FLY) {
		left = _position.x - PARA_GOOMBA_BBOX_WIDTH / 2;
		top = _position.y - PARA_GOOMBA_BBOX_HEIGHT / 2;
		right = left + PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + PARA_GOOMBA_BBOX_HEIGHT;
	}
	else if (STATE_RED_GOOMBA_WALK == _state) {
		left = _position.x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = _position.y - RED_GOOMBA_BBOX_HEIGHT / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT;
	}
}