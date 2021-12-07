#pragma once

#include "CGameObject.h"

#define STATE_PARA_GOOMBA_IDLE 5900
#define STATE_PARA_GOOMBA_WALK 5901
#define STATE_PARA_GOOMBA_FLY 5902
#define STATE_RED_GOOMBA_WALK 5903
#define STATE_RED_GOOMBA_DIE 5904

class CParaGoomba : public CGameObject
{
	int _GetAnimationId();
	float _ax;
	float _ay;
	int _countJump;
	int _warkStartTime;
	ULONGLONG _dieStart;

	void _HandleJump();
	void _HandleStatePara();

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