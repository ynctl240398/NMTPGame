#pragma once

#include "CGameObject.h"
#include "CTimer.h"

#define STATE_KOOPA_TROPA_IDLE 7900
#define STATE_KOOPA_TROPA_WALK 7901
#define STATE_KOOPA_TROPA_SHELD 7902
#define STATE_KOOPA_TROPA_SHELD_RUN 7903
#define STATE_KOOPA_TROPA_SHELD_LIVE 7904
#define STATE_KOOPA_TROPA_FLY 7905

class CKoopaTropa : public CGameObject
{
	CTimer respawnTimer{ true, 5000L };

	int _GetAnimationId();

	bool _flip = false;

	void _Die(LPCOLLISIONEVENT e);

public:
	CKoopaTropa(float x, float y, int state);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z) override;
	int IsBlocking(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

