#include "CKoopaParaTropa.h"
#include "CAnimation.h"
#include "CGame.h"
#include "CDebug.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CGoomba.h"
#include "CItem.h"
#include "CBrickP.h"

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
#define TIME_TO_SHELD_LIVE 3000

CKoopaParaTropa::CKoopaParaTropa(float x, float y, int state){
	_position = { x,y };
	_startPostion = _position;
	_startState = state;
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
		_ay = KOOPA_PARA_TROPA_GRAVITY;
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
		_ay = KOOPA_PARA_TROPA_GRAVITY;
		break;
	case STATE_KOOPA_PARA_TROPA_DIE:
		_liveStart = GetTickCount64();
		break;
	case STATE_KOOPA_PARA_TROPA_IDLE:
		_liveStart = 0;
		_position = _startPostion;
		_ax = 0;
		_ay = 0;
		_velocity = { 0,0 };
		_scale = { -1.0f, 1.0f };
		_obj = new CObjKoopaTropa(_position.x + KOOPA_PARA_TROPA_BBOX_WIDTH / 2 + OBJ_BBOX_WIDTH / 2, _position.y);
		break;
	}
	CGameObject::SetState(state);
}

void CKoopaParaTropa::_SetPositionXObj(float x)
{
	float px, py;

	if (_obj->IsNoCollisionWithPlatform()) {
		_velocity.x = -_velocity.x;
		_scale.x = -_scale.x;
		_obj->SetIsNoCollisionWithPlatform(false);
		py = _position.y;
	}
	else py = _obj->GetPosition().y;

	if (_scale.x < 0) { //right
		px = x + KOOPA_PARA_TROPA_BBOX_WIDTH / 2;
	}
	else {
		px = x - KOOPA_PARA_TROPA_BBOX_WIDTH / 2;
	}
	_obj->SetPosition({ px, py });
}

void CKoopaParaTropa::OnNoCollision(DWORD dt) 
{
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
	else {
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			goomba->SetState(STATE_GOOMBA_DIE);
		}
		if (dynamic_cast<CBrickP*>(e->obj)) {
			CBrickP* brickP = dynamic_cast<CBrickP*>(e->obj);
			if (brickP->GetState() == STATE_BRICK_P_BRICK && e->obj->IsBlocking() && e->nx != 0)
			{
				brickP->SetState(STATE_BRICK_P_BRICK_BREAK);
			}
		}
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj) || dynamic_cast<CBrickP*>(e->obj)) {
			if (dynamic_cast<CBrickQuestion*>(e->obj)) {
				if (_state == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
					CBrickQuestion* brickQuestion = dynamic_cast<CBrickQuestion*>(e->obj);
					if (brickQuestion->GetState() == STATE_BRICK_QUESTION_RUN && e->obj->IsBlocking())
					{
						brickQuestion->SetState(STATE_BRICK_QUESTION_IDLE);
					}
				}
			}
			if (dynamic_cast<CBrick*>(e->obj) && dynamic_cast<CBrick*>(e->obj)->IsBig()) {
				_handleNoCollisionX = true;
			}
			else {
				_velocity.x = -_velocity.x;
				_scale.x = -_scale.x;
			}
		}
	}
}

void CKoopaParaTropa::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
	if (_state == STATE_KOOPA_PARA_TROPA_WALK) {
		_obj->Render();
	}
}

void CKoopaParaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetWindowWidth() + leftCam;

	if (_state == STATE_KOOPA_PARA_TROPA_SHELD_RUN && 
		(_position.x + KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH < leftCam + KOOPA_PARA_TROPA_WIDTH || _position.x - KOOPA_PARA_TROPA_SHELD_BBOX_WIDTH > rightCam)) {
		SetState(STATE_KOOPA_PARA_TROPA_DIE);
	}

	if (_state == STATE_KOOPA_PARA_TROPA_DIE && _liveStart != 0 && GetTickCount64() - _liveStart > TIME_TO_LIVE) {
		SetState(STATE_KOOPA_PARA_TROPA_IDLE);
	}

	if (_state == STATE_KOOPA_PARA_TROPA_IDLE && _position.x >= leftCam && _position.x <= rightCam) {
		SetState(_startState);
	}

	CGameObject::Update(dt, coObjects);

	//handle position obj front
	if (_state == STATE_KOOPA_PARA_TROPA_WALK) {
		_obj->Update(dt, coObjects);
		_SetPositionXObj(_position.x);
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