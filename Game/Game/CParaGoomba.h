#pragma once
#include "CGameObject.h"
#include "CTimer.h"

#define STATE_PARA_GOOMBA_IDLE 5900
#define STATE_PARA_GOOMBA_WALK 5901
#define STATE_PARA_GOOMBA_FLY 5902
#define STATE_RED_GOOMBA_WALK 5903

class CParaGoomba : public CGameObject
{
	int _GetAnimationId();

	int _countJump;

	CTimer _walkTimer{ true, 1000 };

	void _Die(LPCOLLISIONEVENT e);
	void _DieJump(LPCOLLISIONEVENT e);

public:
	CParaGoomba(float x, float y);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z) override;
	int IsBlocking(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};