#pragma once
#include "CGameObject.h"
#include "CFirer.h"
#include "CTimer.h"

#define STATE_VENUS_FIRE_TRAP_IDLE			8900
#define STATE_VENUS_FIRE_TRAP_UP			8901
#define STATE_VENUS_FIRE_TRAP_UP_FIRER		8902
#define STATE_VENUS_FIRE_TRAP_DOWN			8903
#define STATE_VENUS_FIRE_TRAP_DOWN_FIRER	8904
#define STATE_VENUS_FIRE_TRAP_DIE			8905

#define TYPE_VENUS_FIRE_TRAP_RED			8100
#define TYPE_VENUS_FIRE_TRAP_GREEN			8101

#define VENUS_FIRE_TRAP_BBOX_WIDTH			16
#define VENUS_FIRE_TRAP_BBOX_HIEGHT			32

#define TIME_TO_FIRE							2000 //s
#define VENUS_SPEED_Y							0.02f

class CVenusFireTrap : public CGameObject
{
	CTimer _startTimer{true, TIME_TO_FIRE };
	CTimer _fireTimer{true, TIME_TO_FIRE / 2 };
	float _offSetY;
	bool _isUp;
	int _type;
	int _GetAnimationId();

	CFirer* _firer;

public:
	CVenusFireTrap(float x,float y, int type, float offSetY);

	int GetType() { return _type; }
	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return (_state != STATE_VENUS_FIRE_TRAP_DIE);
	}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsBlocking(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

