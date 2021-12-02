#pragma once

#include "CGameObject.h"

#define STATE_PARA_GOOMBA_WALK 5900
#define STATE_PARA_GOOMBA_FLY 5901
#define STATE_RED_GOOMBA_WALK 5902
#define STATE_RED_GOOMBA_DIE 5903

class CParaGoomba : public CGameObject
{
	int _GetAnimationId();
	float _ax;
	float _ay;
	int _countJump;
	int _warkStartTime;
	ULONGLONG _dieStart;
	bool _isOnGround;

public:
	CParaGoomba(float x, float y);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_RED_GOOMBA_DIE);
	}

	void Release() override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};