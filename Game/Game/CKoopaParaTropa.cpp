#include "CKoopaParaTropa.h"
#include "CAnimation.h"
#include "CGame.h"
#include "CDebug.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CGoomba.h"
#include "CItem.h"
#include "CObjKoopaTropa.h"

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

#define KOOPA_PARA_TROPA_GRAVITY 0.002f
#define KOOPA_PARA_TROPA_WALK_SPEED 0.04f
#define KOOPA_PARA_TROPA_SHELD_SPEED 0.2f


#define TIME_TO_LIVE 10000
#define TIME_TO_SHELD_LIVE 5000
#define TIME_TO_SHELD_LIVE_WALK 2000

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
	_ay = KOOPA_PARA_TROPA_GRAVITY;
	_ax = 0;
	switch (state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		_velocity.x = KOOPA_PARA_TROPA_WALK_SPEED;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CKoopaParaTropa::OnNoCollision(DWORD dt) 
{
}

void CKoopaParaTropa::OnCollisionWith(LPCOLLISIONEVENT e) {
	switch (_state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		break;
	default:
		break;
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
	return 0;
}

void CKoopaParaTropa::Render() {
	_aniId = _GetAnimationId();
	_scale.y = _flip ? -1 : 1;
	_scale.x = _velocity.x > 0 ? -1 : 1;
	CGameObject::Render();
	_obj->Render();
}

void CKoopaParaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	if (_state == STATE_KOOPA_PARA_TROPA_IDLE) {
		if (_position.x <= rightCam && _position.x >= leftCam) {
			SetState(STATE_KOOPA_PARA_TROPA_WALK);
		}
	}
	else {
		switch (_state)
		{
		case STATE_KOOPA_PARA_TROPA_WALK:
			_obj->Update(dt, coObjects);
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD:			
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
			break;
		case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
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
	else if (_state == STATE_KOOPA_PARA_TROPA_DIE) {
		left = top = right = bottom = 0;
	}
	else
	{
		left = _position.x - KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH;
		bottom = top + KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT;
	}
}