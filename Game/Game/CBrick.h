#pragma once

#include "CGameObject.h"
#include "CSprite.h"
#include "CAnimation.h"

class CBrick : public CGameObject
{
private:
	int _width;
	int _height;

	bool _isBig;
public:
	CBrick(float x, float y, int w, int h, bool isBig = false);

	bool IsBig() { return this->_isBig; }

	void Load(LPCWSTR);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void Release() override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};