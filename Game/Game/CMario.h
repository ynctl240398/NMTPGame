#pragma once

#include "CGameObject.h"
#include "CTexture.h"
#include "CUtil.h"
#include "CAnimation.h"
#include "CSprite.h"
#include "DefineMario.h"

using namespace std;

class CMario : public CGameObject
{
private:
	BOOLEAN _isSitting;
	float _maxVx;
	
	int _untouchable;
	ULONGLONG _untouchable_start;
	BOOLEAN _isOnPlatform;

	DWORD _dt;

	static CMario* __instance;

	int _level;
	int _direction;
	int _coin;
	int _live;
	int _isRedureAlpha;

	void _HandleKeyDown(int);
	void _HandleKeyUp(int);
	void _HandleAlpha();
	void _HandleKeyState(BYTE* states);

	void _Walk();
	void _Run();
	void _Jump();
	void _ReleaseJump();
	void _Attack();
	void _Sit();
	void _ReleaseSit();
	void _Idle();
	void _Die();
	void _HanldeDie();

	int _GetAnimationId();

	void _UpdateCamPosition();

	void _OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void _OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e);
	void _OnCollisionWithItem(LPCOLLISIONEVENT e);
	void _OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void _OnCollisionWithKoopaParaTropa(LPCOLLISIONEVENT e);
	void _OnCollisionWithKoopaTropa(LPCOLLISIONEVENT e);
	void _OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
	void _OnCollisionWithEnemy(LPCOLLISIONEVENT e);
public:
	CMario();

	void SetState(int);
	static CMario* GetInstance() { 
		if (__instance == NULL) {
			__instance = new CMario;
		}
		return __instance; 
	}

	int GetLevel() { return _level; }
	void SetLevel(int level);
	int IsCollidable()
	{
		return (_state != STATE_MARIO_DIE);
	}

	int IsBlocking() { return (_state != STATE_MARIO_DIE && _untouchable == 0); }

	void Update(DWORD, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	void OnKeyUp(int);
	void OnKeyDown(int);
	void KeyState(BYTE* states);

	void StartUntouchable() { _untouchable = 1; _untouchable_start = GetTickCount64(); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetLive() {
		return _live;
	}

	void ReStart() {
		_position = _startPostion;
		_level = LEVEL_SMALL;
		_live--;
		SetState(STATE_MARIO_IDLE);
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

