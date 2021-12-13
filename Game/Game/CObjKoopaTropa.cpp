#include "CObjKoopaTropa.h"
#include "CBrick.h"
#include "CBrickP.h"

//obj

CObjKoopaTropa::CObjKoopaTropa(float x, float y) {
	_position = { x,y };
	_ay = OBJ_GRAVITY;
	_ax = OBJ_SPEED;
	_velocity = { 0,0 };
	_isNoCollisionWithPlatform = false;
}


void CObjKoopaTropa::OnNoCollision(DWORD dt) {
	_position.y += _velocity.y * dt;
	_isNoCollisionWithPlatform = true;
}

void CObjKoopaTropa::OnCollisionWith(LPCOLLISIONEVENT e) 
{
	
	if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickP*>(e->obj)) {
		if (e->ny != 0)
		{
			_velocity.y = 0;
			_isNoCollisionWithPlatform = false;
		}
	}
	else
		_isNoCollisionWithPlatform = true;
}

void CObjKoopaTropa::Render() {
	RenderBoundingBox();
}

void CObjKoopaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	CGameObject::Update(dt, coObjects);
}


void CObjKoopaTropa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - OBJ_BBOX_WIDTH / 2;
	top = _position.y - OBJ_BBOX_HEIGHT / 2;
	right = left + OBJ_BBOX_WIDTH;
	bottom = top + OBJ_BBOX_HEIGHT;
}