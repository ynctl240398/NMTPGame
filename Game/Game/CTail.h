#pragma once

#include "CGameObject.h"

#define MARIO_TAIL_BBOX_WIDTH		16
#define MARIO_TAIL_BBOX_HEIGHT		10

class CTail : public CGameObject
{
	bool _isShow;

public:
	CTail(float x, float y);

	void SetShow(bool value) {
		_isShow = value;
	}

	bool GetShow() { return _isShow; }

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

