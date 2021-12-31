#include "CKoopaTropa.h"
#include "CItem.h"
#include "CGoomba.h"
#include "CBrickQuestion.h"
#include "CBrick.h"
#include "CAniObject.h"
#include "CMario.h"
#include "CTail.h"
#include "CKoopaParaTropa.h"

#define ID_ANI_KOOPA_TROPA_WALK 7000
#define ID_ANI_KOOPA_TROPA_SHELD 7001
#define ID_ANI_KOOPA_TROPA_SHELD_RUN 7002
#define ID_ANI_KOOPA_TROPA_SHELD_LIVE 7003
#define ID_ANI_KOOPA_TROPA_FLY 7004

#define KOOPA_TROPA_WIDTH 16
#define KOOPA_TROPA_HEIGHT 32

#define KOOPA_TROPA_BBOX_WIDTH 16
#define KOOPA_TROPA_BBOX_HEIGHT 26

#define KOOPA_TROPA_SHELD_BBOX_WIDTH 16
#define KOOPA_TROPA_SHELD_BBOX_HEIGHT 16

#define KOOPA_TROPA_FLY_GRAVITY 0.001f
#define KOOPA_TROPA_GRAVITY 0.0008f
#define KOOPA_TROPA_WALK_SPEED 0.05f
#define KOOPA_TROPA_SHELD_SPEED 0.2f
#define KOOPA_TROPA_JUMP_SPEED 0.4f

#define TIME_TO_LIVE 5000
#define TIME_TO_SHELD_LIVE 3000

void CKoopaTropa::_Die(LPCOLLISIONEVENT e)
{
	if (CMario::GetInstance()->hand == this) {
		CMario::GetInstance()->hand = NULL;
	}

	_isDeleted = true;

	CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.3f, ID_ANI_KOOPA_TROPA_SHELD, { 1.0f, -1.0f });
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
}

CKoopaTropa::CKoopaTropa(float x, float y, int state) {
	_position = { x,y };
	_startPostion = _position;
	_startState = state;
	SetState(STATE_KOOPA_TROPA_IDLE);
}

int CKoopaTropa::_GetAnimationId() {
	int aniId = -1;
	switch (_state)
	{
	case STATE_KOOPA_TROPA_WALK:
		aniId = ID_ANI_KOOPA_TROPA_WALK;
		break;
	case STATE_KOOPA_TROPA_SHELD:
		aniId = ID_ANI_KOOPA_TROPA_SHELD;
		break;
	case STATE_KOOPA_TROPA_SHELD_RUN:
		aniId = ID_ANI_KOOPA_TROPA_SHELD_RUN;
		break;
	case STATE_KOOPA_TROPA_SHELD_LIVE:
		aniId = ID_ANI_KOOPA_TROPA_SHELD_RUN;
		break;
	case STATE_KOOPA_TROPA_FLY:
		aniId = ID_ANI_KOOPA_TROPA_FLY;
		break;
	default:
		aniId = -1;
		break;
	}
	return aniId;
}

void CKoopaTropa::SetState(int state) {
	float l, t, r, b;
	float nl, nt, nr, nb;
	GetBoundingBox(l, t, r, b);

	_ay = KOOPA_TROPA_GRAVITY;
	_ax = 0;
	switch (state)
	{
	case STATE_KOOPA_TROPA_FLY:
		_ay = KOOPA_TROPA_FLY_GRAVITY;
		_velocity.x = KOOPA_TROPA_WALK_SPEED;
		_velocity.y = KOOPA_TROPA_JUMP_SPEED;
		_flip = false;
		break;
	case STATE_KOOPA_TROPA_WALK:
		_velocity.x = KOOPA_TROPA_WALK_SPEED;
		_flip = false;
		break;
	case STATE_KOOPA_TROPA_SHELD_RUN:
		_velocity.x = KOOPA_TROPA_SHELD_SPEED;
		break;
	case STATE_KOOPA_TROPA_SHELD:
		_velocity.x = 0;
		respawnTimer.SetTimeOut(TIME_TO_LIVE);
		respawnTimer.Reset();
		respawnTimer.Start();
		break;
	case STATE_KOOPA_TROPA_SHELD_LIVE:
		_velocity.x = 0;
		respawnTimer.SetTimeOut(TIME_TO_SHELD_LIVE);
		respawnTimer.Reset();
		respawnTimer.Start();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);

	GetBoundingBox(nl, nt, nr, nb);
	_position.y -= (nb - b);
}

void CKoopaTropa::OnNoCollision(DWORD dt)
{
	_position -= _velocity * (FLOAT)dt;
	_position.x += _velocity.x * dt;
	_position.y += (_velocity.y * dt) / 2.25f;
}

void CKoopaTropa::OnCollisionWith(LPCOLLISIONEVENT e) {
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	switch (_state)
	{
	case STATE_KOOPA_TROPA_FLY:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_KOOPA_TROPA_WALK);
			}
		}
		break;
	case STATE_KOOPA_TROPA_WALK:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_KOOPA_TROPA_SHELD);
				AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
			}
		}
		break;
	case STATE_KOOPA_TROPA_SHELD_RUN:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_KOOPA_TROPA_SHELD);
				AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
			}
		}
		break;
	case STATE_KOOPA_TROPA_SHELD:
	case STATE_KOOPA_TROPA_SHELD_LIVE:
		if (e->obj == CMario::GetInstance()) {
			if (!kb->IsKeyDown(DIK_A) || e->ny > 0) {
				SetState(STATE_KOOPA_TROPA_SHELD_RUN);
				_velocity.x *= CMario::GetInstance()->_direction;
			}
		}
		break;
	default:
		break;
	}

	CKoopaParaTropa* paraKoopa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (paraKoopa) {
		if (paraKoopa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == paraKoopa) {
			_Die(e);
		}
	}

	CKoopaTropa* koopa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (koopa) {
		if (koopa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == koopa) {
			_Die(e);
		}
	}

	if (dynamic_cast<CTail*>(e->obj)) {
		SetState(STATE_KOOPA_TROPA_SHELD);
		_velocity.y = -0.28f;
		_flip = true;
	}
}

void CKoopaTropa::OnBlockingOn(bool isHorizontal, float z)
{
	if (isHorizontal) {
		_velocity.x *= -1;
	}
	else {
		if (_state == STATE_KOOPA_TROPA_FLY && z < 0) {
			_velocity.y = -KOOPA_TROPA_JUMP_SPEED;
		}
		else {
			_velocity.y = 0;
		}
	}
}

int CKoopaTropa::IsBlocking(LPCOLLISIONEVENT e)
{
	if (e->obj == CMario::GetInstance()) {
		if (e->ny > 0) {
			return true;
		}
		return _state != STATE_KOOPA_TROPA_WALK;
	}
	return dynamic_cast<CKoopaTropa*>(e->obj) == nullptr;
}

void CKoopaTropa::Render() {
	_aniId = _GetAnimationId();
	_scale.y = _flip ? -1.0f : 1.0f;
	_scale.x = _velocity.x > 0 ? -1.0f : 1.0f;
	CGameObject::Render();
}

void CKoopaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	respawnTimer.Update(dt);

	if (_state == STATE_KOOPA_TROPA_IDLE) {
		if (_position.x <= rightCam && _position.x >= leftCam) {
			SetState(_startState);
			_velocity.x *= -1;
		}
	}
	else {
		switch (_state)
		{
		case STATE_KOOPA_TROPA_FLY:
			break;
		case STATE_KOOPA_TROPA_WALK:
			if (CMario::GetInstance()->hand == this) {
				CMario::GetInstance()->hand = NULL;
				SetState(STATE_KOOPA_TROPA_WALK);
				_position.y -= 6;
			}
			break;
		case STATE_KOOPA_TROPA_SHELD_RUN:
			break;
		case STATE_KOOPA_TROPA_SHELD:
			if (respawnTimer.GetState() == CTimerState::TIMEOVER) {
				SetState(STATE_KOOPA_TROPA_SHELD_LIVE);
			}
			break;
		case STATE_KOOPA_TROPA_SHELD_LIVE:
			if (respawnTimer.GetState() == CTimerState::TIMEOVER) {
				SetState(STATE_KOOPA_TROPA_WALK);
			}
			break;
		default:
			break;
		}

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}	
}

void CKoopaTropa::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_state == STATE_KOOPA_TROPA_WALK || _state == STATE_KOOPA_TROPA_FLY)
	{
		left = _position.x - KOOPA_TROPA_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_TROPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_TROPA_BBOX_WIDTH;
		bottom = top + KOOPA_TROPA_BBOX_HEIGHT;
	}
	else
	{
		left = _position.x - KOOPA_TROPA_SHELD_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_TROPA_SHELD_BBOX_HEIGHT / 2;
		right = left + KOOPA_TROPA_SHELD_BBOX_WIDTH;
		bottom = top + KOOPA_TROPA_SHELD_BBOX_HEIGHT;
	}
}