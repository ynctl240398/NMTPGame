#include "CItem.h"
#include "CUtil.h"
#include "CBrickQuestion.h"
#include "CBrick.h"
#include "CMario.h"
#include "CBrickCoin.h"

#define ITEM_GRAVITY		0.0015f
#define ITEM_RUN_SPEED		0.07f
#define ITEM_JUMP_SPEED		0.15f
#define ITEM_FALL_X			0.000095f
#define ITEM_FALL_Y			0.025f
#define ITEM_FALL_X_MAX		0.07f
#define ITEM_UP_SPEED		0.08f
#define ITEM_LEAF_JUMP		0.4f

CItem::CItem(float x, float y, string type) {
	_position = { x,y };
	_velocity = { 0, 0 };
	_isActive = false;
	_startPostion = _position;
	_ay = 0;
	_ax = 0;
	_isDeleted = false;
	_step = false;
	SetType(type);
	SetState(STATE_ITEM_IDLE);
}

void CItem::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (_type == TYPE_ITEM_P) {
		if (_state != STATE_ITEM_P_PUSHED) {
			if (e->ny > 0) {
				if (e->obj == CMario::GetInstance()) {
					float l, t, r, b;
					float nl, nt, nr, nb;
					GetBoundingBox(l, t, r, b);
					_state = STATE_ITEM_P_PUSHED;
					GetBoundingBox(nl, nt, nr, nb);
					_position.y -= nb - b;

					for (LPGAMEOBJECT var : _coObjects)
					{
						CBrickCoin* brickCoin = dynamic_cast<CBrickCoin*>(var);
						if (brickCoin) {
							brickCoin->Transform();
						}
					}
				}
			}
		}
	}
	else {
		if (e->obj == CMario::GetInstance()) {
			if (_type == TYPE_ITEM_MUSHROOM_GREEN || _type == TYPE_ITEM_MUSHROOM_RED || _type == TYPE_ITEM_LEAF) {
				AddPointAni(ID_ANI_POINT_1000, _position.x, _position.y);
			}

			_isDeleted = true;
		}

		if (_type == TYPE_ITEM_MUSHROOM_GREEN) {

		}
		else if (_type == TYPE_ITEM_MUSHROOM_RED) {

		}
	}
}

void CItem::OnBlockingOn(bool isHorizontal, float z)
{
	if (_type == TYPE_ITEM_MUSHROOM_GREEN || _type == TYPE_ITEM_MUSHROOM_RED) {
		if (isHorizontal) {
			_velocity.x *= -1;
		}
		else {
			_velocity.y = 0;
		}
	}
}

void CItem::OnNoCollision(DWORD dt) 
{
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float w = r - l;
	float h = b - t;

	_coObjects.insert(_coObjects.end(), coObjects->begin(), coObjects->end());

	if (_type == TYPE_ITEM_P) {
		if (_state != STATE_ITEM_P_PUSHED) {
			if (abs(_position.y - _startPostion.y) < h) {
				_position.y -= ITEM_UP_SPEED * dt;
			}
			else {
				_position.y = _startPostion.y - h;
			}

			float ml, mt, mr, mb, sl, st, sr, sb;
			GetBoundingBox(ml, mt, mr, mb);
			CMario::GetInstance()->GetBoundingBox(sl, st, sr, sb);
			if (CCollision::IsOverlap(ml, mt, mr, mb, sl, st, sr, sb)) {
				LPCOLLISIONEVENT e = new CCollisionEvent(0.01f, 1, 0, -1, 0, 0, this, CMario::GetInstance());
				OnCollisionWith(e);
			}
		}
	}
	else if (_type == TYPE_ITEM_COIN_BRICK) {
		if (_step == 0) {
			if (abs(_position.y - _startPostion.y) < 32) {
				_position.y -= ITEM_JUMP_SPEED * dt;
			}
			else {
				_step = 1;
			}
		}
		else if (_step == 1) {
			if (_position.y < _startPostion.y - h) {
				_position.y += ITEM_JUMP_SPEED * dt;
			}
			else {
				_step = 2;
				_isDeleted = true;
				AddPointAni(ID_ANI_POINT_100, _position.x, _position.y);
			}
		}
	}
	else if (_type == TYPE_ITEM_MUSHROOM_GREEN || _type == TYPE_ITEM_MUSHROOM_RED) {
		if (_step == 0) {
			if (abs(_position.y - _startPostion.y) < h) {
				_position.y -= ITEM_UP_SPEED * dt;
			}
			else {
				_step = 1;
				_ay = ITEM_GRAVITY;
			}
		}
		else if (_step == 1) {
			_velocity.x = CMario::GetInstance()->GetPosition().x < _position.x ? ITEM_RUN_SPEED : ITEM_RUN_SPEED * -1;
			_step = 2;
		}
	}
	else if (_type == TYPE_ITEM_LEAF) {
		if (_step == 0) {
			_velocity.y = -ITEM_LEAF_JUMP;
			_ay = ITEM_GRAVITY / 1.5;
			_step = 1;
		}
		else if (_step == 1) {
			if (_velocity.y > 0) {
				_ax = -ITEM_FALL_X;
				_velocity.x += _ax * dt;
				_ay = 0;
				_velocity.y = ITEM_FALL_Y;
				_step = 2;
			}
		}
		else if (_step == 2) {
			if (abs(_velocity.x) > ITEM_FALL_X_MAX) {
				_velocity.x = 0;
				_ax = -_ax;
			}
		}
	}

	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);

	_coObjects.clear();
}

void CItem::SetType(string type) {
	_type = type;
}

int CItem::IsBlocking(LPCOLLISIONEVENT e)
{
	return _type == TYPE_ITEM_P && _state != STATE_ITEM_P_PUSHED;
}

int CItem::_GetAnimationId() {
	int aniId;

	if (_state == STATE_ITEM_DISAPPEAR) {
		return -1;
	}

	if (_type == TYPE_ITEM_P) {
		if (_state == STATE_ITEM_P_PUSHED) {
			aniId = ID_ANI_ITEM_P_PUSHED;
		}
		else {
			aniId = ID_ANI_ITEM_P;
		}
	} else if (_type == TYPE_ITEM_COIN_BRICK) {
		aniId = ID_ANI_ITEM_COIN_BRICK;
	}
	else if (_type == TYPE_ITEM_MUSHROOM_GREEN) {
		aniId = ID_ANI_ITEM_MUSHROOM_GREEN;
	}
	else if (_type == TYPE_ITEM_MUSHROOM_RED) {
		aniId = ID_ANI_ITEM_MUSHROOM_RED;
	}
	else if (_type == TYPE_ITEM_LEAF) {
		aniId = ID_ANI_ITEM_LEAF;
	}
	else aniId = -1;

	return aniId;
}

void CItem::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	float w = _state == STATE_ITEM_P_PUSHED ? ITEM_WIDTH : ITEM_WIDTH;
	float h = _state == STATE_ITEM_P_PUSHED ? ITEM_HEIGHT : ITEM_HEIGHT;

	left = _position.x - w / 2;
	top = _position.y - h / 2;
	right = left + w;
	bottom = top + h;
}

CItem::~CItem() {

}