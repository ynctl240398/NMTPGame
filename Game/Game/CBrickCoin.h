#pragma once
#include "CGameObject.h"
#include "CTimer.h"
#include "CUtil.h"
#include "CDebug.h"

#define BRICK_COIN_WIDTH 16
#define BRICK_COIN_HEIGHT 16

#define ID_ANI_BRICK_COIN_COIN 3002
#define ID_ANI_BRICK_COIN_BRICK 11000


#define STATE_BRICK_P_BRICK					11900
#define STATE_BRICK_P_IDLE					11901
#define STATE_BRICK_P_PUSHED				11902
#define STATE_BRICK_P_BRICK_UP				11903
#define STATE_BRICK_P_BRICK_BREAK			11904
#define STATE_BRICK_P_COIN					11905

#define BRICK_P_BBOX_WIDTH					16
#define BRICK_P_BBOX_HEIGHT					16

#define BRICK_BROKEN_BBOX_WIDTH				8
#define BRICK_BROKEN_BBOX_HEIGHT			8

#define BRICK_BROKEN_GRAVITY				0.002f
#define BRICK_BROKEN_SPEED_X				0.1f
#define BRICK_BROKEN_JUMP_DEFLECT_SPEED		0.5f

#define ID_ANI_PARTICLES_BRICK_BROKEN_IDLE	12000

class CBrickCoin :
    public CGameObject
{
protected:
	CTimer transferTimer{ true, 8000 };

	bool _brick;

	int _GetAnimationId();

	void _TranformUpdate(DWORD dt);

	void _SpawnBreakEffect();

public:
    CBrickCoin(float x, float y, bool brick);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsBlocking(LPCOLLISIONEVENT e);

	void Transform();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

