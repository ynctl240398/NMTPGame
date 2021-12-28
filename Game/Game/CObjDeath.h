#pragma once

#include "CGameObject.h"

class CObjDeath : public CGameObject
{
private:
	int _width;
	int _height;
public:
	CObjDeath(float x, float y, int w, int h);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsBlocking(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

