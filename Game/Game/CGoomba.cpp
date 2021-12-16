#include "CGoomba.h"
#include "CAnimation.h"
#include "CItem.h"
#include "CBrick.h"
#include "CBrickQuestion.h"

#define GOOMBA_WIDTH 16
#define GOOMBA_HEIGHT 15

#define GOOMBA_BBOX_HEIGHT_DIE 8

#define ID_ANI_GOOMBA_WALK 4000
#define ID_ANI_GOOMBA_DIE 4001

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALK_SPEED 0.05f

CGoomba::CGoomba(float x, float y) {
	_position = { x,y };
	_velocity = { 0.0f,0.0f };
	_ay = GOOMBA_GRAVITY;
	_ax = 0;
	_dieStart = -1;
	_state = STATE_GOOMBA_IDLE;
}

int CGoomba::_GetAnimationId() {
	int aniId = -1;
	if (_state == STATE_GOOMBA_WALK) {
		aniId = ID_ANI_GOOMBA_WALK;
	}
	else if (_state == STATE_GOOMBA_DIE) aniId = ID_ANI_GOOMBA_DIE;
	return aniId;
}

void CGoomba::SetState(int state) {
	switch (state)
	{
	case STATE_GOOMBA_DIE:
		_dieStart = GetTickCount64();
		_position.y += (GOOMBA_WIDTH - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		_velocity = { 0.0f,0.0f };
		_ay = 0;
		_ax = 0;
		break;
	case STATE_GOOMBA_WALK:
		_velocity.x = -GOOMBA_WALK_SPEED;
		break;
	case STATE_GOOMBA_IDLE:
		break;
	}
	CGameObject::SetState(state);
}

void CGoomba::OnNoCollision(DWORD dt) {
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CItem*>(e->obj)) {
		_handleNoCollisionX = true;
		return;
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			_velocity.x = -_velocity.x;
			goomba->SetVelocity({ -goomba->GetVelocity().x, goomba->GetVelocity().y });
		}
		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) {
			if (dynamic_cast<CBrick*>(e->obj) && dynamic_cast<CBrick*>(e->obj)->IsBig()) {

				_handleNoCollisionX = true;
			}
			else
				_velocity.x = -_velocity.x;
		}
	}
}

void CGoomba::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	float leftWindow = CCam::GetInstance()->GetPosition().x;
	float rightWindow = CGame::GetInstance()->GetWindowWidth() + leftWindow;

	if (_state == STATE_GOOMBA_IDLE && leftWindow <= _position.x && rightWindow >= _position.x) {
		SetState(STATE_GOOMBA_WALK);
	}

	if ((_state == STATE_GOOMBA_DIE) && (GetTickCount64() - _dieStart > GOOMBA_DIE_TIMEOUT))
	{
		_isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_state == STATE_GOOMBA_DIE)
	{
		left = _position.x - GOOMBA_WIDTH / 2;
		top = _position.y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = _position.x - GOOMBA_WIDTH / 2;
		top = _position.y - GOOMBA_HEIGHT / 2;
		right = left + GOOMBA_WIDTH;
		bottom = top + GOOMBA_HEIGHT;
	}
}