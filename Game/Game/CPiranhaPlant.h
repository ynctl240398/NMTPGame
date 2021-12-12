#pragma once
#include "CGameObject.h"

#define STATE_PIRANHA_PLANT_IDLE			9900
#define STATE_PIRANHA_PLANT_UP				9901
#define STATE_PIRANHA_PLANT_DIE				9902

#define TYPE_PIRANHA_PLANT_RED				9100
#define TYPE_PIRANHA_PLANT_GREEN			9101

#define PIRANHA_PLANT_BBOX_WIDTH			16
#define PIRANHA_PLANT_BBOX_HIEGHT			32

class CPiranhaPlant: public CGameObject
{
	ULONGLONG _startTime;
	float _offSetY;
	bool _isUp;
	int _type;
	int _GetAnimationId();

public:
	CPiranhaPlant(float x, float y, int type, float offSetY);

	int GetType() { return _type; }
	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_PIRANHA_PLANT_DIE);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

