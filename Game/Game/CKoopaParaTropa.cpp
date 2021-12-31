#include "CKoopaParaTropa.h"
#include "CAnimation.h"
#include "CGame.h"
#include "CDebug.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CGoomba.h"
#include "CItem.h"
#include "CObjKoopaTropa.h"
#include "CMario.h"
#include "CTail.h"
#include "CKeyBoard.h"	
#include "CAniObject.h"
#include "CKoopaTropa.h"

#define ID_ANI_KOOPA_PARA_TROPA_WALK 6000
#define ID_ANI_KOOPA_PARA_TROPA_SHELD 6001
#define ID_ANI_KOOPA_PARA_TROPA_SHELD_RUN 6002
#define ID_ANI_KOOPA_PARA_TROPA_SHELD_LIVE 6003
#define ID_ANI_KOOPA_PARA_TROPA_FLY 6004

#define KOOPA_PARA_TROPA_WIDTH 16
#define KOOPA_PARA_TROPA_HEIGHT 32

#define KOOPA_PARA_TROPA_BBOX_WIDTH 16
#define KOOPA_PARA_TROPA_BBOX_HEIGHT 26

#define KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH 16
#define KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT 14

#define KOOPA_PARA_TROPA_GRAVITY 0.0008f
#define KOOPA_PARA_TROPA_WALK_SPEED 0.04f
#define KOOPA_PARA_TROPA_SHELD_SPEED 0.2f


#define TIME_TO_LIVE 10000
#define TIME_TO_SHELD_LIVE 5000
#define TIME_TO_SHELD_LIVE_WALK 2000

void CKoopaParaTropa::_Die(LPCOLLISIONEVENT e)
{
	if (CMario::GetInstance()->hand == this) {
		CMario::GetInstance()->hand = NULL;
	}

	_isDeleted = true;

	CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.3f, ID_ANI_KOOPA_PARA_TROPA_SHELD, {1.0f, -1.0f});
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
}

CKoopaParaTropa::CKoopaParaTropa(float x, float y, int state){
	_position = { x,y };
	_startPostion = _position;
	_startState = state;
	_obj = new CObjKoopaTropa(this);
	SetState(STATE_KOOPA_PARA_TROPA_IDLE);
}

int CKoopaParaTropa::_GetAnimationId() {
	int aniId = -1;
	switch (_state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		aniId = ID_ANI_KOOPA_PARA_TROPA_WALK;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
		aniId = ID_ANI_KOOPA_PARA_TROPA_SHELD;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:		
		aniId = ID_ANI_KOOPA_PARA_TROPA_SHELD_RUN;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		aniId = ID_ANI_KOOPA_PARA_TROPA_SHELD_LIVE;
		break;
	case STATE_KOOPA_PARA_TROPA_FLY:
		aniId = ID_ANI_KOOPA_PARA_TROPA_FLY;
		break;
	default:
		aniId = -1;
		break;
	}
	return aniId;
}

void CKoopaParaTropa::SetState(int state) {
	float l, t, r, b;
	float nl, nt, nr, nb;
	GetBoundingBox(l, t, r, b);

	_ay = KOOPA_PARA_TROPA_GRAVITY;
	_ax = 0;
	switch (state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		_velocity.x = KOOPA_PARA_TROPA_WALK_SPEED;
		_flip = false;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
		_velocity.x = KOOPA_PARA_TROPA_SHELD_SPEED;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
		_velocity.x = 0;
		respawnTimer.SetTimeOut(TIME_TO_SHELD_LIVE);
		respawnTimer.Reset();
		respawnTimer.Start();
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		_velocity.x = 0;
		respawnTimer.SetTimeOut(TIME_TO_SHELD_LIVE_WALK);
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

void CKoopaParaTropa::OnNoCollision(DWORD dt) 
{
}

void CKoopaParaTropa::OnCollisionWith(LPCOLLISIONEVENT e) {
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	switch (_state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_KOOPA_PARA_TROPA_SHELD);
				AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
			}
		}
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
		if (e->obj == CMario::GetInstance()) {
			if (e->ny > 0) {
				SetState(STATE_KOOPA_PARA_TROPA_SHELD);
				AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
			}
		}
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		if (e->obj == CMario::GetInstance()) {
			if (!kb->IsKeyDown(DIK_A) || e->ny > 0) {
				SetState(STATE_KOOPA_PARA_TROPA_SHELD_RUN);
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
		SetState(STATE_KOOPA_PARA_TROPA_SHELD);
		_velocity.y = -0.28f;
		_flip = true;
		AddAttackEffect(_position.x, _position.y);
	}
}

void CKoopaParaTropa::OnBlockingOn(bool isHorizontal, float z)
{
	if (isHorizontal) {
		_velocity.x *= -1;
	}
	else {
		_velocity.y = 0;
	}
}

int CKoopaParaTropa::IsBlocking(LPCOLLISIONEVENT e)
{
	if (e->obj == CMario::GetInstance()) {
		if (e->ny > 0) {
			return true;
		}
		return _state != STATE_KOOPA_PARA_TROPA_WALK;
	}
	return dynamic_cast<CKoopaParaTropa*>(e->obj) == nullptr;
}

void CKoopaParaTropa::Render() {
	_aniId = _GetAnimationId();
	_scale.y = _flip ? -1.0f : 1.0f;
	_scale.x = _velocity.x > 0 ? -1.0f : 1.0f;
	CGameObject::Render();
	_obj->Render();
}

void CKoopaParaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	respawnTimer.Update(dt);

	if (_state == STATE_KOOPA_PARA_TROPA_IDLE) {
		if (_position.x <= rightCam && _position.x >= leftCam) {
			SetState(_startState);
		}
	}
	else {
		switch (_state)
		{
		case STATE_KOOPA_PARA_TROPA_WALK:
			if (CMario::GetInstance()->hand == this) {
				CMario::GetInstance()->hand = NULL;
				SetState(STATE_KOOPA_PARA_TROPA_WALK);
				_position.y -= 6;
			}
			else {
				_obj->Update(dt, coObjects);
			}
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD:	
			if (respawnTimer.GetState() == CTimerState::TIMEOVER) {
				SetState(STATE_KOOPA_PARA_TROPA_SHELD_LIVE);
			}
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
			if (respawnTimer.GetState() == CTimerState::TIMEOVER) {
				SetState(STATE_KOOPA_PARA_TROPA_WALK);
			}
			break;
		default:
			break;
		}

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CKoopaParaTropa::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_state == STATE_KOOPA_PARA_TROPA_WALK || _state == STATE_KOOPA_PARA_TROPA_FLY)
	{
		left = _position.x - KOOPA_PARA_TROPA_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_PARA_TROPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARA_TROPA_BBOX_WIDTH;
		bottom = top + KOOPA_PARA_TROPA_BBOX_HEIGHT;
	}
	else
	{
		left = _position.x - KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH;
		bottom = top + KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT;
	}
}