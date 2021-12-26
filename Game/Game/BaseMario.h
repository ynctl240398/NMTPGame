#pragma once
#include "CUtil.h"
#include "CCollision.h"
#include "CAnimation.h"
#include "DefineMario.h"

class CMario;

class BaseMario
{
protected:
	CMario* mario;

	unordered_map<int, LPANIMATION> animations;	

	MarioState _stateId;

	virtual void _WalkUpdate(DWORD dt);
	virtual void _JumpUpdate(DWORD dt);
	virtual void _SitUpdate(DWORD dt);
	virtual void _PositionUpdate(DWORD dt);

	int	  PMETER_MAX = 7;
	float PMETER_UP_STEP = 0.008f;
	float PMETER_DOWN_STEP = 0.0016f;
	float MARIO_WALK_SPEED = 0.27f / 3;
	float MARIO_RUN_SPEED = 0.48f / 3;

	float MARIO_WALK_ACCELERATION = 0.000376f / 3;
	float MARIO_RUN_ACCELERATION = 0.0003613f / 3;
	float MARIO_WALK_DRAG_FORCE = 0.0014306f / 3;
	float MARIO_RUN_DRAG_FORCE = 0.0016366f / 3;
	float MARIO_CROUCH_DRAG_FORCE = 0.0008766f / 3;
	float MARIO_SKID_ACCELERATION = 0.001104f / 3;

	float MARIO_GRAVITY = 0.002f / 2;
	float MARIO_MIN_JUMP_HEIGHT = 80 / 3;
	float MARIO_JUMP_HEIGHT = 97 / 3;
	float MARIO_PUSH_FORCE = 0.432f / 3;

	float MARIO_MIN_HIGH_JUMP_HEIGHT = 97 / 3;
	float MARIO_HIGH_JUMP_HEIGHT = 216 / 3;
	float MARIO_SUPER_JUMP_HEIGHT = 300 / 3;
	float MARIO_SUPER_PUSH_FORCE = 0.632f / 3;

	float MAX_FLY_SPEED = 999.0f / 3;
public:	
	BaseMario(CMario* mario, MarioState stateId);

	virtual void AnimationInit();

	virtual void Update(DWORD, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void OnBlockingOn(bool isHorizontal, float z);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	virtual MarioState GetStateID();
};

