#pragma once

#include "CGameObject.h"
#include "CSprite.h"
#include "CAnimation.h"

class CBrick : public CGameObject
{
private:
	int _width;
	int _height;
public:
	CBrick(float x, float y, int w, int h);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};