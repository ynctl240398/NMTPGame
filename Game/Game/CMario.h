#pragma once

#include "CGameObject.h"
#include "CTexture.h"
#include "CUtil.h"
#include "CAnimation.h"
#include "CSprite.h"

using namespace std;

class CMario : public CGameObject
{
private:
	int _state;
	int _currentAniId;
	
	float _runSpeed;
	float _jumpSpeed;

	static CMario* __instance;

	string _level;
	int _direction;
	CTextures* _tex;
	CAnimation* _ani;

	void _ParseSectionSprites(string);
	void _ParseSectionFrames(string);
	void _ParseSectionAnimation(string);

	void _HandleKeyboard();
	void _HandleState();

	void _Move();
	void _Jump();
	void _Attack();
	void _Sit();
	void _Idle();
	void _Die();
	void _HanldeDie();

	void _UpdateCamPosition();
public:
	CMario();
	static CMario* GetInstance() { 
		if (__instance == NULL) {
			__instance = new CMario;
		}
		return __instance; 
	}
	void Load(string);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};

