#include "CGoomba.h"
#include "CAnimation.h"
#include "CItem.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CMario.h"
#include "CAniObject.h"
#include "CTail.h"
#include "CKoopaParaTropa.h"
#include "CKoopaTropa.h"

#define GOOMBA_WIDTH			16
#define GOOMBA_HEIGHT			15

#define GOOMBA_BBOX_HEIGHT_DIE	8

#define ID_ANI_GOOMBA_WALK		4000
#define ID_ANI_GOOMBA_DIE		4001
#define ID_ANI_GOOMBA_DIE_JUMP	4002

#define GOOMBA_DIE_TIMEOUT		500

#define GOOMBA_GRAVITY			0.0009f
#define GOOMBA_WALK_SPEED		0.05f
#define GOOMBA_JUMP_DIE			0.5f

CGoomba::CGoomba(float x, float y) {
	_position = { x,y };
	_velocity = { 0.0f,0.0f };
	_ay = GOOMBA_GRAVITY;
	_ax = 0;
	_state = STATE_GOOMBA_IDLE;
}

void CGoomba::SetState(int state) {
	switch (state)
	{
	case STATE_GOOMBA_WALK:
		_velocity.x = -GOOMBA_WALK_SPEED;
		break;
	case STATE_GOOMBA_IDLE:
		break;
	}
	CGameObject::SetState(state);
}

int CGoomba::_GetAnimationId() {
	return ID_ANI_GOOMBA_WALK;
}

void CGoomba::OnNoCollision(DWORD dt) {
	
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny > 0) {
		if (dynamic_cast<CMario*>(e->obj)) {
			_isDeleted = true;

			CAniObject* aniObj = new CAniObject({ _position.x, _position.y + 4}, 0, 0, ID_ANI_GOOMBA_DIE, { 1, 1 }, 700L, 0.0f);
			CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
			AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
		}
	}
	
	CTail* tail = dynamic_cast<CTail*>(e->obj);
	if (tail) {
		_isDeleted = true;

		CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.25f, ID_ANI_GOOMBA_DIE_JUMP, { 1, -1 });
		CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
		AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
		AddAttackEffect(_position.x, _position.y);
	}

	CKoopaParaTropa* paraKoopa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (paraKoopa) {
		if (paraKoopa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == paraKoopa) {
			_isDeleted = true;

			CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.25f, ID_ANI_GOOMBA_DIE_JUMP, { 1, -1 });
			CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
			AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
		}
	}

	CKoopaTropa* koopa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (koopa) {
		if (koopa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN || CMario::GetInstance()->hand == koopa) {
			_isDeleted = true;

			CAniObject* aniObj = new CAniObject(_position, 0.02f * e->nx, -0.25f, ID_ANI_GOOMBA_DIE_JUMP, { 1, -1 });
			CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
			AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
		}
	}
}

void CGoomba::OnBlockingOn(bool isHorizontal, float z)
{
	if (isHorizontal) {
		_velocity.x *= -1;
	}
	else {
		_velocity.y = 0;
	}
}

int CGoomba::IsBlocking(LPCOLLISIONEVENT e)
{
	if (e->obj == CMario::GetInstance()) {
		if (e->ny > 0) {
			return true;
		}
	}
	return dynamic_cast<CGoomba*>(e->obj) == nullptr;
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

	CGameObject::Update(dt, coObjects);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - GOOMBA_WIDTH / 2;
	top = _position.y - GOOMBA_HEIGHT / 2;
	right = left + GOOMBA_WIDTH;
	bottom = top + GOOMBA_HEIGHT;
}