#pragma once

#include "CGameObject.h"

#define STATE_GOOMBA_WALK 4900
#define STATE_GOOMBA_DIE 4901

class CGoomba : public CGameObject
{
	int _GetAnimationId();
	float _ax;
	float _ay;

	ULONGLONG _dieStart;

public:
	CGoomba(float x, float y);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_GOOMBA_DIE);
	}

	void Release() override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

