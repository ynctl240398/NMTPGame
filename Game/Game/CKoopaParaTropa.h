#pragma once

#include "CGameObject.h"
#include "CObjKoopaTropa.h"

#define STATE_KOOPA_PARA_TROPA_IDLE 6900
#define STATE_KOOPA_PARA_TROPA_WALK 6901
#define STATE_KOOPA_PARA_TROPA_SHELD 6902
#define STATE_KOOPA_PARA_TROPA_SHELD_RUN 6903
#define STATE_KOOPA_PARA_TROPA_SHELD_LIVE 6904
#define STATE_KOOPA_PARA_TROPA_FLY 6905
#define STATE_KOOPA_PARA_TROPA_DIE 6906


class CKoopaParaTropa : public CGameObject
{

	ULONGLONG _liveStart;
	ULONGLONG _liveSheldStart;
	ULONGLONG _liveSheldWalkStart;

	CObjKoopaTropa* _obj;

	int _GetAnimationId();

	void _SetPositionXObj(float x);
public:
	CKoopaParaTropa(float x, float y, int state);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_KOOPA_PARA_TROPA_DIE);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};