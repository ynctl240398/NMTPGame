#include "CParaGoomba.h"
#include "CAnimation.h"
#include "CBrickQuestion.h"
#include "CBrick.h"
#include "CItem.h"
#include "CGoomba.h"
#include "CDebug.h"

#define PARA_GOOMBA_BBOX_WIDTH 20
#define PARA_GOOMBA_BBOX_HEIGHT 24

#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 16

#define RED_GOOMBA_BBOX_HEIGHT_DIE 8

#define ID_PARA_GOOMBA_WALK_ANI 5000
#define ID_PARA_GOOMBA_FLY_ANI 5001
#define ID_RED_GOOMBA_WALK_ANI 5002
#define ID_RED_GOOMBA_DIE_ANI 5003

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_GRAVITY 0.001f
#define GOOMBA_WALK_SPEED 0.03f
#define PARA_GOOMBA_WALK_SPEED 0.015f
#define PARA_GOOMBA_FLY_SPEED 0.4f
#define PARA_GOOMBA_FLY_SPEED_SHORT 0.15f

#define MAX_COUNT_JUMP 4
#define MAX_TIME_WALK 500

CParaGoomba::CParaGoomba(float x, float y) {
	_position = { x,y };
	_ax = 0;
	_ay = GOOMBA_GRAVITY;
	_countJump = 0;
	_warkStartTime = 0;
	_dieStart = -1;
	_velocity = { 0.0f,0.0f };
	_state = -1;
}

int CParaGoomba::_GetAnimationId() {
	int aniId = -1;
	switch (_state)
	{
	case STATE_RED_GOOMBA_DIE:
		aniId = ID_RED_GOOMBA_DIE_ANI;
		break;
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
	_warkStartTime = 0;
	switch (state)
	{
	case STATE_RED_GOOMBA_DIE:
		_dieStart = GetTickCount64();
		_position.y += (RED_GOOMBA_BBOX_WIDTH - RED_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		_velocity = { 0.0f,0.0f };
		_ay = 0;
		_ax = 0;
		break;
	case STATE_RED_GOOMBA_WALK:
		_velocity.x = _velocity.x == 0 ? -GOOMBA_WALK_SPEED : _velocity.x;
		break;
	case STATE_PARA_GOOMBA_FLY:
		_velocity.x = -PARA_GOOMBA_WALK_SPEED;
		if (_countJump < MAX_COUNT_JUMP) {
			_velocity.y = -PARA_GOOMBA_FLY_SPEED_SHORT;
			_countJump++;
		}
		else {
			_velocity.y = -PARA_GOOMBA_FLY_SPEED;
			_countJump++;
			
		}
		break;
	case STATE_PARA_GOOMBA_WALK:
		_warkStartTime = GetTickCount64();
		_velocity.x = -GOOMBA_WALK_SPEED;
		break;
	}
	CGameObject::SetState(state);
}

void CParaGoomba::OnNoCollision(DWORD dt) {
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt / 2;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CItem*>(e->obj)) {
		_handleNoCollisionX = true;
		return;
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
		if (e->ny < 0) { 
			_isOnGround = true;  
			if (_countJump > MAX_COUNT_JUMP ) {
				if (_warkStartTime == 0) {
					SetState(STATE_PARA_GOOMBA_WALK);
				}
			}
			else {
				SetState(STATE_PARA_GOOMBA_FLY);
			}
		}
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<CBrick*>(e->obj) && dynamic_cast<CBrick*>(e->obj)->IsBig()) {
			_handleNoCollisionX = true;
		}
		else {
			_velocity.x = -_velocity.x;
		}
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	if ((_state == STATE_GOOMBA_DIE) && (GetTickCount64() - _dieStart > GOOMBA_DIE_TIMEOUT))
	{
		_isDeleted = true;
		return;
	}

	int leftWindow = CCam::GetInstance()->GetPosition().x;
	int rightWindow = CGame::GetInstance()->GetWindowHeight() * 2.25f + leftWindow;

	if (_state == -1 && leftWindow <= _position.x && rightWindow >= _position.x) {
		SetState(STATE_PARA_GOOMBA_FLY);
	}

	if (_warkStartTime != 0 && GetTickCount64() - _warkStartTime >= MAX_TIME_WALK) {
		_countJump = 0;
		_warkStartTime = 0;
		SetState(STATE_PARA_GOOMBA_FLY);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParaGoomba::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CParaGoomba::Release() {

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
	else {
		left = _position.x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = _position.y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT_DIE;
	}
}