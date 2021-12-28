#pragma once

#include "CGameObject.h"

#define PORTAL_WIDTH	8
#define PORTAL_HEIGHT	32

class CPortalOutSmall : public CGameObject
{
	int _direction; // down -> 1, up -> -1

public:
	CPortalOutSmall(float x, float y, int direction);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CPortalOutSmall();
};

