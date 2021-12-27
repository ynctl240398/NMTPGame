#pragma once

#include "CGameObject.h"

#define STATE_FIRER_FLY			9900
#define STATE_FIRER_JUMP		9901
#define STATE_FIRER_DISAPPEAR	9902

class CFirer : public CGameObject
{
	int _GetAnimationId();
public:
	CFirer(float x, float y);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void SetState(int) override;

	int IsCollidable()
	{
		return (_state != STATE_FIRER_DISAPPEAR);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int IsBlocking(LPCOLLISIONEVENT e) { return 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

