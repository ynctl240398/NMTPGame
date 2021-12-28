#pragma once
#include "CUtil.h"
#include "CGameObject.h"
#include "CTexture.h"
#include "CUtil.h"
#include "CAnimation.h"
#include "CSprite.h"
#include "DefineMario.h"
#include "CTimer.h"

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
	int _coin;
	int _live;
	int _isRedureAlpha;
	bool _untouchable = false;

	bool _tele = false;

	float _jumpStartHeight = 0;

	float _drag = 0;

	float _direction = 1;

	LPGAMEOBJECT hand = nullptr;

	float powerMeter = 0;

	D3DXVECTOR2 accelerate;

	bool controllable = true;

	bool sliding = false;

	MarioState marioState;

	MarioWalkState walkState;

	MarioJumpState jumpState;

	CTimer* _kickTimer;

	CTimer _untouchableTimer{ true, 3000 };

	static CMario* GetInstance();

	CMario();
	
	void Update(DWORD, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	void SetOnGround(bool);
	bool IsOnGround();

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z);

	int IsBlocking(LPCOLLISIONEVENT e);

	void Die();
	void SheldUpdate(DWORD dt);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

