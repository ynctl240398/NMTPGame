#pragma once

#include "CGameObject.h"

#define STATE_KOOPA_TROPA_IDLE 7900
#define STATE_KOOPA_TROPA_WALK 7901
#define STATE_KOOPA_TROPA_SHELD 7902
#define STATE_KOOPA_TROPA_SHELD_RUN 7903
#define STATE_KOOPA_TROPA_SHELD_LIVE 7904
#define STATE_KOOPA_TROPA_FLY 7905
#define STATE_KOOPA_TROPA_DIE 7906

class CKoopaTropa : public CGameObject
{

	ULONGLONG _liveStart;

	int _GetAnimationId();

public:
	CKoopaTropa(float x, float y, int state);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_KOOPA_TROPA_DIE);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

