#include "CItem.h"
#include "CUtil.h"
#include "CBrickQuestion.h"
#include "CBrick.h"
#include "CMario.h"

#define ITEM_GRAVITY 0.002f
#define ITEM_RUN_SPEED 0.05f
#define ITEM_JUMP_SPEED 0.015f
#define MAX_JUMP 0.75f

CItem::CItem(float x, float y, string type) {
	_position = { x,y };
	_isActive = false;
	_startY = y;
	_maxVy = MAX_JUMP;
	_ay = 0;
	_ax = 0;
	_isDeleted = false;
	_state = -1;
	SetType(type);
	SetState(STATE_ITEM_IDLE);
}

void CItem::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;

	if (dynamic_cast<CMario*>(e->obj)) {
		
	}

	if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) {
		
	}
	else {
		_handleNoCollisionX = true;
		return;
	}

	if (e->ny != 0)
	{
		_velocity.y = 0;
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) 
		{
			if (dynamic_cast<CBrick*>(e->obj) && dynamic_cast<CBrick*>(e->obj)->IsBig()) {
				_handleNoCollisionX = true;
			}
			else {
				_velocity.x = -_velocity.x;
			}
		}
	}
}

void CItem::OnNoCollision(DWORD dt) {
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt / 2.5;
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (_isDeleted) return;

	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	if (abs(_velocity.y) >= _maxVy) {
		_ay = JUMP_SPEED;
	}

	float dy = _velocity.y * dt / 4;

	if (_state == STATE_ITEM_JUMP) {
		if (_position.y + dy >= _startY) {
			_position.y = _startY;
			_ay = 0;
			Delete();
			SetState(STATE_ITEM_DISAPPEAR);
		}
		else
			_position.y += dy;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (_handleNoCollisionX) {
		_handleNoCollisionX = false;
		_position.x += _velocity.x * dt;
	}
}

void CItem::SetType(string type) {
	_type = type;
}

void CItem::SetState(int state) {
	if (state == STATE_ITEM_JUMP) {
		_ay = -JUMP_SPEED;
	}
	else if (state == STATE_ITEM_MOVE) {

	}
	else if (state == STATE_ITEM_IDLE) {

	}
	else if (state == STATE_ITEM_DISAPPEAR) {

	}
	
	CGameObject::SetState(state);
}

int CItem::_GetAnimationId() {
	int aniId;

	if (_state == STATE_ITEM_DISAPPEAR) {
		return -1;
	}

	if (_type == TYPE_ITEM_COIN) {
		aniId = ID_ITEM_COIN_ANI;
	}
	else if (_type == TYPE_ITEM_MUSHROOM_GREEN) {
		aniId = ID_ITEM_MUSHROOM_GREEN_ANI;
	}
	else if (_type == TYPE_ITEM_MUSHROOM_RED) {
		aniId = ID_ITEM_MUSHROOM_RED_ANI;
	}
	else aniId = -1;

	return aniId;
}

void CItem::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();

	LPANIMATION animation = animations->Get(aniId);

	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - ITEM_WIDTH / 2;
	top = _position.y - ITEM_HEIGHT / 2;
	right = left + ITEM_WIDTH;
	bottom = top + ITEM_HEIGHT;
}

void CItem::Release() {

}

CItem::~CItem() {

}