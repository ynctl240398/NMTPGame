#pragma once

#include "CGameObject.h"
#include "CTimer.h"

#define MARIO_TAIL_BBOX_WIDTH		2
#define MARIO_TAIL_BBOX_HEIGHT		2
#define MARIO_TAIL_RANGE_FRONT		14.5f
#define MARIO_TAIL_RANGE_BACK		12.0f

class CTail : public CGameObject
{
	CTimer _liveTimer{ true, 60 };

	int _step;

public:
	CTail();

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

