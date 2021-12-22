#pragma once

#include "CGameObject.h"

#define STATE_GOOMBA_IDLE		4900
#define STATE_GOOMBA_WALK		4901
#define STATE_GOOMBA_DIE		4902
#define STATE_GOOMBA_DIE_JUMP	4903

class CGoomba : public CGameObject
{
	int _GetAnimationId();

	ULONGLONG _dieStart;

public:
	CGoomba(float x, float y);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_GOOMBA_DIE && _state != STATE_GOOMBA_DIE_JUMP);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

