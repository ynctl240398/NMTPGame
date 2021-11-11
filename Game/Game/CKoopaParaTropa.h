#pragma once

#include "CGameObject.h"

#define STATE_KOOPA_PARA_TROPA_WALK 6900
#define STATE_KOOPA_PARA_TROPA_SHELD 6901
#define STATE_KOOPA_PARA_TROPA_SHELD_RUN 6902
#define STATE_KOOPA_PARA_TROPA_SHELD_LIVE 6903
#define STATE_KOOPA_PARA_TROPA_FLY 6904
#define STATE_KOOPA_PARA_TROPA_DIE 6905

class CKoopaParaTropa : public CGameObject
{
	float _ax;
	float _ay;
	float _maxPx;
	float _minPx;

	int _GetAnimationId();
public:
	CKoopaParaTropa(float x, float y, int state, float maxPx, float minPx);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_KOOPA_PARA_TROPA_DIE);
	}

	void Release() override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

