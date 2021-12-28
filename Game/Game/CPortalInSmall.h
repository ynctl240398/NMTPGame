#pragma once

#include "CGameObject.h"

#define PORTAL_WIDTH	8
#define PORTAL_HEIGHT	32

class CPortalInSmall : public CGameObject
{

	int _direction; // down -> 1, up -> -1

	float _cx;
	float _cy;



public:
	CPortalInSmall(float x, float y, int direction, float cx, float cy);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CPortalInSmall();
};

