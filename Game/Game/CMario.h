#pragma once
#include "CUtil.h"
#include "CGameObject.h"
#include "CTexture.h"
#include "CUtil.h"
#include "CAnimation.h"
#include "CSprite.h"
#include "DefineMario.h"

using namespace std;

class BaseMario;

class CMario : public CGameObject
{
private:
	static CMario* __instance;

	BaseMario* stateMachine;

	bool onGround = true;

	void _UpdateState();

	void _CheatPointUpdate();

public:
	float _jumpStartHeight = 0;

	float _drag = 0;

	int _direction = 1;

	float powerMeter = 0;

	D3DXVECTOR2 accelerate;

	bool controllable = true;

	bool sliding = false;

	int skid = 0;

	MarioState marioState;

	MarioWalkState walkState;

	MarioJumpState jumpState;

	static CMario* GetInstance();

	CMario();
	
	void Update(DWORD, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	void SetOnGround(bool);
	bool IsOnGround();

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

