#include "CKoopaTropa.h"
#include "CItem.h"
#include "CGoomba.h"
#include "CBrickQuestion.h"
#include "CBrick.h"

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
#define KOOPA_TROPA_GRAVITY 0.002f
#define KOOPA_TROPA_WALK_SPEED 0.05f
#define KOOPA_TROPA_SHELD_SPEED 0.2f
#define KOOPA_TROPA_JUMP_SPEED 0.4f

#define TIME_TO_LIVE 5000
#define TIME_TO_SHELD_LIVE 3000

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
	switch (state)
	{
	case STATE_KOOPA_TROPA_WALK:
		_ay = KOOPA_TROPA_GRAVITY;
		_velocity.x = -KOOPA_TROPA_WALK_SPEED * _scale.x;
		break;
	case STATE_KOOPA_TROPA_SHELD:
		_velocity.x = 0;
		break;
	case STATE_KOOPA_TROPA_SHELD_RUN:
		_velocity.x = -KOOPA_TROPA_SHELD_SPEED * _scale.x;
		break;
	case STATE_KOOPA_TROPA_SHELD_LIVE:
		break;
	case STATE_KOOPA_TROPA_FLY:
		_ay = KOOPA_TROPA_FLY_GRAVITY;
		_velocity.y = -KOOPA_TROPA_JUMP_SPEED;
		_velocity.x = -(KOOPA_TROPA_WALK_SPEED / 1.5f)* _scale.x;
		break;
	case STATE_KOOPA_TROPA_DIE:
		_liveStart = GetTickCount64();
		break;
	case STATE_KOOPA_TROPA_IDLE:
		_liveStart = 0;
		_position = _startPostion;
		_ax = 0;
		_ay = 0;
		_velocity = { 0,0 };
		_scale = { 1.0f, 1.0f };
		break;
	}
	CGameObject::SetState(state);
}

void CKoopaTropa::OnNoCollision(DWORD dt)
{
	_position -= _velocity * dt;
	_position.x += _velocity.x * dt;
	_position.y += (_velocity.y * dt) / 2.25f;
}

void CKoopaTropa::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking(e)) return;

	if (dynamic_cast<CItem*>(e->obj)) {
		_handleNoCollisionX = true;
		return;
	}

	if (_state != STATE_KOOPA_TROPA_SHELD_RUN) {
		if (dynamic_cast<CKoopaTropa*>(e->obj) || dynamic_cast<CGoomba*>(e->obj)) {
			_handleNoCollisionX = true;
			_handleNoCollisionY = true;
			return;
		}
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
		if (e->ny < 0 && _state == STATE_KOOPA_TROPA_FLY) {
			SetState(_state);
		}
	}
	else if (e->nx != 0)
	{
		if (_state == STATE_KOOPA_TROPA_SHELD_RUN) {
			if (dynamic_cast<CGoomba*>(e->obj)) {
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != STATE_GOOMBA_DIE) {
					goomba->SetState(STATE_GOOMBA_DIE);
				}
				else {
					_handleNoCollisionX = true;
				}
			}
			if (dynamic_cast<CBrickQuestion*>(e->obj)) {
				CBrickQuestion* brickQuestion = dynamic_cast<CBrickQuestion*>(e->obj);
				if (brickQuestion->GetState() == STATE_BRICK_QUESTION_RUN && e->obj->IsBlocking(e))
				{
					brickQuestion->SetState(STATE_BRICK_QUESTION_IDLE);
				}
			}
		}
		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) {
			_velocity.x = -_velocity.x;
			_scale.x = -_scale.x;
		}
	}
}

void CKoopaTropa::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CKoopaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	if (_state == STATE_KOOPA_TROPA_DIE && _liveStart != 0 && GetTickCount64() - _liveStart > TIME_TO_LIVE) {
		SetState(STATE_KOOPA_TROPA_IDLE);
	}

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	float topCam = CCam::GetInstance()->GetPosition().y;
	float bottomCam = CGame::GetInstance()->GetWindowHeight() + topCam;

	if (_state == STATE_KOOPA_TROPA_SHELD_RUN &&
		(_position.x + KOOPA_TROPA_SHELD_BBOX_WIDTH < leftCam + KOOPA_TROPA_WIDTH || _position.x - KOOPA_TROPA_SHELD_BBOX_WIDTH > rightCam)) {
		SetState(STATE_KOOPA_TROPA_DIE);
	}

	if (_state == STATE_KOOPA_TROPA_IDLE && _position.x >= leftCam && _position.x <= rightCam) {
		SetState(_startState);
	}

	CGameObject::Update(dt, coObjects);
}

void CKoopaTropa::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_state == STATE_KOOPA_TROPA_WALK || _state == STATE_KOOPA_TROPA_FLY)
	{
		left = _position.x - KOOPA_TROPA_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_TROPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_TROPA_BBOX_WIDTH;
		bottom = top + KOOPA_TROPA_BBOX_HEIGHT;
	}
	else if (_state == STATE_KOOPA_TROPA_DIE) {
		left = top = right = bottom = 0;
	}
	else
	{
		left = _position.x - KOOPA_TROPA_SHELD_BBOX_WIDTH / 2;
		top = _position.y - KOOPA_TROPA_SHELD_BBOX_HEIGHT / 2;
		right = left + KOOPA_TROPA_SHELD_BBOX_WIDTH;
		bottom = top + KOOPA_TROPA_SHELD_BBOX_HEIGHT;
	}
}