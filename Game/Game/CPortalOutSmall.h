#pragma once

#include "CGameObject.h"

class CPortalOutSmall : public CGameObject
{
	int _direction; // down -> 1, up -> -1

public:
	float _w;
	float _h;

	CPortalOutSmall(float x, float y, float w, float h, int direction);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	virtual int IsBlocking(LPCOLLISIONEVENT e) { return 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CPortalOutSmall();
};

