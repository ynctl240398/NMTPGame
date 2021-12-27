#pragma once

#include "CGameObject.h"
#include "CTimer.h"

#define STATE_KOOPA_PARA_TROPA_IDLE 6900
#define STATE_KOOPA_PARA_TROPA_WALK 6901
#define STATE_KOOPA_PARA_TROPA_SHELD 6902
#define STATE_KOOPA_PARA_TROPA_SHELD_RUN 6903
#define STATE_KOOPA_PARA_TROPA_SHELD_LIVE 6904
#define STATE_KOOPA_PARA_TROPA_FLY 6905

class CObjKoopaTropa;

class CKoopaParaTropa : public CGameObject
{
	CTimer respawnTimer{ true, 5000L };

	CObjKoopaTropa* _obj;

	int _GetAnimationId();

	bool _flip = false;

public:
	CKoopaParaTropa(float x, float y, int state);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z) override;
	int IsBlocking(LPCOLLISIONEVENT e) override;

	void _Die(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};