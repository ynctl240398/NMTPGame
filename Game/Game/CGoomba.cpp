#include "CGoomba.h"
#include "CAnimation.h"

#define GOOMBA_WIDTH 16
#define GOOMBA_HEIGHT 16

#define GOOMBA_BBOX_HEIGHT_DIE 8

#define ID_GOOMBA_ANI_WALK 4000
#define ID_GOOMBA_ANI_DIE 4001

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALK_SPEED 0.05f

CGoomba::CGoomba(float x, float y) {
	_position = { x,y };
	_velocity = { 0.0f,0.0f };
	_ay = GOOMBA_GRAVITY;
	_ax = 0;
	_state = -1;
	_dieStart = -1;
}

int CGoomba::_GetAnimationId() {
	int aniId = -1;
	if (_state == STATE_GOOMBA_WALK) {
		aniId = ID_GOOMBA_ANI_WALK;
	}
	else if (_state == STATE_GOOMBA_DIE) aniId = ID_GOOMBA_ANI_DIE;
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
	}
	CGameObject::SetState(state);
}

void CGoomba::OnNoCollision(DWORD dt) {
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		_velocity.y = 0;
	}
	else if (e->nx != 0)
	{
		_velocity.x = -_velocity.x;
	}
}

void CGoomba::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	_velocity.y += _ay * dt;
	_velocity.x += _ax * dt;

	int leftCam = CCam::GetInstance()->GetCameraBound()->GetLeft();
	int rightCam = CCam::GetInstance()->GetCameraBound()->GetRight();

	if (_position.x >= leftCam - GOOMBA_WIDTH / 2 && _position.x < rightCam - GOOMBA_WIDTH / 2) {
		if (_state != STATE_GOOMBA_DIE) {
			SetState(STATE_GOOMBA_WALK);
		}
	}
	else if (_position.x < leftCam) { _ax = 0; this->Delete(); }

	if ((_state == STATE_GOOMBA_DIE) && (GetTickCount64() - _dieStart > GOOMBA_DIE_TIMEOUT))
	{
		_isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoomba::Release() {
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