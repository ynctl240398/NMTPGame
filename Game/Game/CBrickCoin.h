#pragma once
#include "CGameObject.h"
#include "CTimer.h"
#include "CUtil.h"
#include "CDebug.h"

#define BRICK_COIN_WIDTH 16
#define BRICK_COIN_HEIGHT 16

#define ID_ANI_BRICK_COIN_COIN 3002
#define ID_ANI_BRICK_COIN_BRICK 11000

class CBrickCoin :
    public CGameObject
{
protected:
	CTimer transferTimer{ true, 8000 };

	bool _brick;

	int _GetAnimationId();

	void _TranformUpdate(DWORD dt);

public:
    CBrickCoin(float x, float y, bool brick);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsBlocking(LPCOLLISIONEVENT e);

	void Transform();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

