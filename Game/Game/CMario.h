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

	string _level;
	string _direction;
	CTextures* _tex;
	CAnimation* _ani;

	void _ParseSectionSprites(string);
	void _ParseSectionAni(string);
	void _ParseSectionAnis(string);
public:
	CMario();

	void Load(string);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};

