#include "CKoopaParaTropa.h"
#include "CAnimation.h"
#include "CGame.h"
#include "CDebug.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CGoomba.h"
#include "CItem.h"

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
#define KOOPA_PARA_TROPA_SHELD_BBOX_HEIGHT 16

#define KOOPA_PARA_TROPA_GRAVITY 0.002f
#define KOOPA_PARA_TROPA_WALK_SPEED 0.05f
#define KOOPA_PARA_TROPA_SHELD_SPEED 0.2f

CKoopaParaTropa::CKoopaParaTropa(float x, float y, int state, float maxPx, float minPx){
	_position = { x,y };
	_ax = 0;
	_maxPx = maxPx;
	_minPx = minPx;
	_ay = KOOPA_PARA_TROPA_GRAVITY;
	_velocity = { 0,0 };
	_scale = { -1.0f, 1.0f };
	SetState(state);
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
		aniId = ID_ANI_KOOPA_PARA_TROPA_SHELD_RUN;
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
	switch (state)
	{
	case STATE_KOOPA_PARA_TROPA_WALK:
		_velocity.x = -KOOPA_PARA_TROPA_WALK_SPEED * _scale.x;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD:
		_velocity.x = 0;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_RUN:
		_velocity.x = -KOOPA_PARA_TROPA_SHELD_SPEED * _scale.x;
		break;
	case STATE_KOOPA_PARA_TROPA_SHELD_LIVE:
		break;
	case STATE_KOOPA_PARA_TROPA_FLY:
		break;
	}
	CGameObject::SetState(state);
}

void CKoopaParaTropa::OnNoCollision(DWORD dt) {
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
}

void CKoopaParaTropa::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;

	if (dynamic_cast<CItem*>(e->obj)) {
		_handleNoCollisionX = true;
		return;
	}

	if (_state != STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
		if (dynamic_cast<CKoopaParaTropa*>(e->obj) || dynamic_cast<CGoomba*>(e->obj)) {
			_handleNoCollisionX = true;
			return;
		}
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) {
			if (dynamic_cast<CBrick*>(e->obj) && dynamic_cast<CBrick*>(e->obj)->IsBig()) {
				_handleNoCollisionX = true;
			}
			else {
				_velocity.x = -_velocity.x;
				_scale.x = -_scale.x;
			}
		}
		if (_state == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
			if (dynamic_cast<CGoomba*>(e->obj)) {
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != STATE_GOOMBA_DIE) {
					goomba->SetState(STATE_GOOMBA_DIE);
				}
				else {
					_handleNoCollisionX = true;
				}
			}
		}
	}
}

void CKoopaParaTropa::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CKoopaParaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	int leftCam = CCam::GetInstance()->GetCameraBound()->GetLeft();
	int rightCam = CCam::GetInstance()->GetCameraBound()->GetRight();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (_state == STATE_KOOPA_PARA_TROPA_WALK) {
		if (_position.x <= _minPx || _position.x >= _maxPx - KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH) {
			_velocity.x = -_velocity.x;
			_scale.x = -_scale.x;
		}
	}

	if (_handleNoCollisionX) {
		_handleNoCollisionX = false;
		_position.x += _velocity.x * dt;
	}
}

void CKoopaParaTropa::Release() {
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